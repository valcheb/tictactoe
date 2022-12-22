#include "common.h"

static uint8_t cell_length;
static point_t field_start;

void cm_set_cell_length(uint8_t length)
{
    cell_length = length;
}

uint8_t cm_get_cell_length(void)
{
    return cell_length;
}

void cm_set_field_start(uint8_t x, uint8_t y)
{
    field_start.x = x;
    field_start.y = y;
}

point_t cm_get_field_start(void)
{
    return field_start;
}
