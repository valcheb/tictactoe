#include "game_screen.h"
#include <stdbool.h>
#include <string.h>

static uint8_t gs_hit_cell(point_t point)
{
    bool hrange = false;
    bool vrange = false;
    uint8_t row = 0;
    uint8_t col = 0;
    point_t start       = cm_get_field_start();
    uint8_t cell_length = cm_get_cell_length();

    for (int i = 0; i < CELL_AMOUNT; i++)
    {
        row = i / 3;
        col = i - row * 3;

        hrange = point.x > start.x + col * cell_length && point.x < start.x + (col + 1) * cell_length;
        vrange = point.y > start.y + row * cell_length && point.y < start.y + (row + 1) * cell_length;

        if (hrange && vrange) //press point belongs to cell
        {
            return i;
        }
    }

    return CELL_AMOUNT;
}

void gs_press_control(const em_arg_t *in) //EM_EVENT_PRESS
{
    point_t  point;
    em_arg_t out;
    bool     hrange = false;
    bool     vrange = false;
    uint8_t  cell   = CELL_AMOUNT;
    point_t  start       = cm_get_field_start();
    uint8_t  cell_length = cm_get_cell_length();

    if (in == NULL)
    {
        return ;
    }
    memcpy(&point, in->data, in->size);

    hrange = point.x > start.x && point.x < start.x + 3 * cell_length;
    vrange = point.y > start.y && point.y < start.y + 3 * cell_length;
    if (hrange && vrange) //press point belongs to field
    {
        cell = gs_hit_cell(point);
        if (cell < CELL_AMOUNT)
        {
            out.size = sizeof(cell);
            memcpy(out.data, &cell, out.size);
            em_emit(EM_EVENT_CELL_CALCULATED, &out);
        }
    }
}
