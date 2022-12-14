#include "common.h"

static uint8_t cell_length;
static point_t field_start;
static point_t player_turn_pos;

void cm_set_cell_length(uint16_t length)
{
    cell_length = length;
}

uint8_t cm_get_cell_length(void)
{
    return cell_length;
}

void cm_set_field_start(uint16_t x, uint16_t y)
{
    field_start.x = x;
    field_start.y = y;
}

point_t cm_get_field_start(void)
{
    return field_start;
}

void cm_set_player_turn_pos(uint16_t x, uint16_t y)
{
    player_turn_pos.x = x;
    player_turn_pos.y = y;
}

point_t cm_get_player_turn_pos(void)
{
    return player_turn_pos;
}

point_t cm_cell_to_point(uint8_t cell)
{
    point_t res;
    uint8_t row = cell / 3;
    uint8_t col = cell - row * 3;

    res.x = field_start.x + cell_length / 2 + col * cell_length;
    res.y = field_start.y + cell_length / 2 + row * cell_length;

    return res;
}
