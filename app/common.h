#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>

typedef enum
{
    PLAYER_EMPTY = 0,
    PLAYER_1,
    PLAYER_2
} player_e;

typedef struct
{
    uint16_t x;
    uint16_t y;
} point_t;

//TODO doc
void cm_set_cell_length(uint16_t length);
uint8_t cm_get_cell_length(void);
void cm_set_field_start(uint16_t x, uint16_t y);
point_t cm_get_field_start(void);
void cm_set_player_turn_pos(uint16_t x, uint16_t y);
point_t cm_get_player_turn_pos(void);

#endif /*COMMON_H_*/
