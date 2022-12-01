#ifndef EVENT_RING_H_
#define EVENT_RING_H_

#include "event_manager.h" //for em_event_t
#include <stdbool.h>
#include <stdint.h>

typedef struct em_relem_s
{
    em_event_t name;
    em_arg_t   arg;
} em_relem_t;

typedef struct em_ring_s
{
    em_relem_t *buffer;
    uint16_t    size;
    uint16_t    current_size;
    uint16_t    in;
    uint16_t    out;
} em_ring_t;

/**
 * @brief Set ring buffer fields to initial values.
 *
 * @param ring    pointer to ring buffer.
 * @param src_buf memory storage for ring buffer.
 * @param size    size of ring buffer.
 */
inline static void er_init(em_ring_t *ring, em_relem_t *src_buf, uint16_t size)
{
    ring->buffer       = src_buf;
    ring->in           = 0;
    ring->out          = 0;
    ring->current_size = 0;
    ring->size         = size;
}

/**
 * @brief Put element to end of ring buffer.
 *
 * @param ring pointer to ring buffer.
 * @param elem element to ring buffer.
 */
inline static void er_push(em_ring_t *ring, em_relem_t elem)
{
    ring->buffer[ring->in] = elem;
    ring->current_size++;

    ring->in++;
    if (ring->in >= ring->size)
    {
        ring->in = 0;
    }
}

/**
 * @brief Extract element from beginning of ring buffer.
 *
 * @param ring[in]  pointer to ring buffer.
 * @param elem[out] element from ring buffer.
 */
inline static void er_pop(em_ring_t *ring, em_relem_t *elem)
{
    *elem = ring->buffer[ring->out];
    ring->current_size--;

    ring->out++;
    if (ring->out >= ring->size)
    {
        ring->out = 0;
    }
}

/**
 * @brief Check if ring buffer is full.
 *
 * @param ring   pointer to ring buffer.
 * @return true
 * @return false
 */
inline static bool er_is_full(em_ring_t *ring)
{
    return ring->current_size >= ring->size;
}

/**
 * @brief Check if ring buffer is empty.
 *
 * @param ring   pointer to ring buffer.
 * @return true
 * @return false
 */
inline static bool er_is_empty(em_ring_t *ring)
{
    return ring->current_size == 0;
}

#endif /*EVENT_RING_H_*/
