#include "event_manager.h"
#include "event_ring.h"
#include <stddef.h>
#include <string.h>

#define EVENT_DICTIONARY_SIZE 10
#define EVENT_RING_SIZE 10

static struct
{
    em_pair_t  em_dictionary[EVENT_DICTIONARY_SIZE];
    em_ring_t  em_ring;
    em_relem_t em_ring_buffer[EVENT_RING_SIZE];
    uint32_t   search_idx;
    uint32_t   dict_curr_size;
} em_ctx;

/**
 * @brief Search index of event in dictionary.
 *
 * @param search_name search event name.
 * @param search_idx  search start index.
 * @return uint32_t
 */
static uint32_t em_find(em_event_t search_name, uint32_t search_idx)
{
    while (search_idx < EVENT_DICTIONARY_SIZE)
    {
        if (em_ctx.em_dictionary[search_idx].event == search_name)
        {
            break;
        }
        search_idx++;
    }

    return search_idx; //return EVENT_DICTIONARY_SIZE if event not found
}

static bool em_is_pair_exist(em_event_t event, em_callback_t callback)
{
    uint32_t idx = 0;

    while (idx < EVENT_DICTIONARY_SIZE)
    {
        if (event    == em_ctx.em_dictionary[idx].event &&
            callback == em_ctx.em_dictionary[idx].callback)
        {
            return true;
        }

        idx++;
    }

    return false;
}

void em_init(void)
{
    er_init(&em_ctx.em_ring, em_ctx.em_ring_buffer, EVENT_RING_SIZE);
    em_ctx.search_idx     = 0;
    em_ctx.dict_curr_size = 0;
}

void em_subscribe(em_event_t event, em_callback_t callback)
{
    if (em_ctx.dict_curr_size >= EVENT_DICTIONARY_SIZE)
    {
        //TODO how to message errors?
        return ;
    }

    if (em_is_pair_exist(event, callback))
    {
        return ;
    }

    em_ctx.em_dictionary[em_ctx.dict_curr_size].event    = event;
    em_ctx.em_dictionary[em_ctx.dict_curr_size].callback = callback;
    em_ctx.dict_curr_size++;
}

void em_emit(em_event_t event, em_arg_t arg)
{
    em_relem_t elem = {.name = event, .arg = arg};

    if (er_is_full(&em_ctx.em_ring))
    {
        return ;
    }

    er_push(&em_ctx.em_ring, elem);
}

void em_handler(void)
{
    static em_relem_t elem = {.name = EM_EVENT_UNDEFINED};

    if (elem.name == EM_EVENT_UNDEFINED && !er_is_empty(&em_ctx.em_ring))
    {
        er_pop(&em_ctx.em_ring, &elem);
    }

    em_ctx.search_idx = em_find(elem.name, em_ctx.search_idx);
    if (em_ctx.search_idx < EVENT_DICTIONARY_SIZE)
    {
        if (em_ctx.em_dictionary[em_ctx.search_idx].callback != NULL)
        {
            em_ctx.em_dictionary[em_ctx.search_idx].callback(elem.arg);
        }

        em_ctx.search_idx++;
    }
    else
    {
        em_ctx.search_idx = 0;
        elem.name         = EM_EVENT_UNDEFINED;
    }
}
