#include "event_manager.h"
#include "event_ring.h"
#include <stddef.h>

#define EVENT_DICTIONARY_SIZE 10
#define EVENT_RING_SIZE 10

static em_pair_t  em_dictionary[EVENT_DICTIONARY_SIZE];
static em_ring_t  em_ring;
static em_relem_t em_ring_buffer[EVENT_RING_SIZE];

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
        if (em_dictionary[search_idx].event == search_name)
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
        if (event    == em_dictionary[idx].event &&
            callback == em_dictionary[idx].callback)
        {
            return true;
        }

        idx++;
    }

    return false;
}

void em_init(void)
{
    er_init(&em_ring, em_ring_buffer, EVENT_RING_SIZE);
}

void em_subscribe(em_event_t event, em_callback_t callback)
{
    static uint32_t dict_curr_size = 0;

    if (dict_curr_size >= EVENT_DICTIONARY_SIZE)
    {
        //TODO how to message errors?
        return ;
    }

    if (em_is_pair_exist(event, callback))
    {
        return ;
    }

    em_dictionary[dict_curr_size].event    = event;
    em_dictionary[dict_curr_size].callback = callback;
    dict_curr_size++;
}

void em_emit(em_event_t event, em_arg_t arg)
{
    em_relem_t elem = {.name = event, .arg = arg};

    if (er_is_full(&em_ring))
    {
        return ;
    }

    er_push(&em_ring, elem);
}

void em_handler(void)
{
    static uint32_t search_idx = 0;
    static em_relem_t elem = {.name = EM_EVENT_UNDEFINED};

    if (elem.name == EM_EVENT_UNDEFINED && !er_is_empty(&em_ring))
    {
        er_pop(&em_ring, &elem);
    }

    search_idx = em_find(elem.name, search_idx);
    if (search_idx < EVENT_DICTIONARY_SIZE)
    {
        if (em_dictionary[search_idx].callback != NULL)
        {
            em_dictionary[search_idx].callback(elem.arg);
        }

        search_idx++;
    }
    else
    {
        search_idx = 0;
        elem.name  = EM_EVENT_UNDEFINED;
    }
}
