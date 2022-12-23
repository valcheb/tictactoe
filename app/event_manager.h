#ifndef EVENT_MANAGER_H_
#define EVENT_MANAGER_H_

#include <stdint.h>

typedef enum
{
    EM_EVENT_UNDEFINED = 0,
    EM_EVENT_PRESS,
    EM_EVENT_CELL_CALCULATED,
    EM_EVENT_PLAYER_TURN,
    EM_EVENT_INIT,
    EM_EVENT_NEW_TURN
} em_event_t;

#define EVENT_ARG_SIZE 1024
typedef struct em_arg_s
{
    uint8_t  data[EVENT_ARG_SIZE];
    uint32_t size;
} em_arg_t;

typedef void (*em_callback_t)(const em_arg_t *arg);

typedef struct em_pair_s
{
    em_event_t    event;
    em_callback_t callback;
} em_pair_t;

/**
 * @brief Init storage for event queue.
 *
 */
void em_init(void);

/**
 * @brief Write pair of event name and event callback function in dictionary.
 *
 * @param event    event name to dictionary.
 * @param callback callback function to dictionary.
 */
void em_subscribe(em_event_t event, em_callback_t callback);

/**
 * @brief Write event name and event argument to event queue.
 *
 * @param event event name to queue.
 * @param arg   argument for callback to queue.
 */
void em_emit(em_event_t event, const em_arg_t *arg);

/**
 * @brief Handle events from event queue.
 *
 * em_handler checks if event queue is full and call callback function for found events, one per handler call.
 * em_handler must be called in main loop.
 */
void em_handler(void);

/**
 * @brief Clear internal event manager buffers and counters
 *
 */
void em_clear(void);

#endif /*EVENT_MANAGER_H_*/
