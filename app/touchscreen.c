#include "touchscreen.h"
#include "common.h"
#include <stdbool.h>
#include <string.h>

static TS_StateTypeDef TS_State;

void ts_init(void)
{
    BSP_TS_Init(FT5336_MAX_WIDTH, FT5336_MAX_HEIGHT);
    BSP_TS_ITConfig();
}

static uint8_t ts_hit_cell(point_t point)
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

void ts_press_handler(const em_arg_t *in) //EM_EVENT_PRESS
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
        cell = ts_hit_cell(point);
        if (cell < CELL_AMOUNT)
        {
            out.size = sizeof(cell);
            memcpy(out.data, &cell, out.size);
            em_emit(EM_EVENT_CELL_CALCULATED, &out);
        }
    }
}

void EXTI15_10_IRQHandler(void)
{
    //__HAL_GPIO_EXTI_GET_IT returns PR register value by mask TS_INT_PIN
    //this value, shifted to the right by a number of bits equal to the exti line number,
    //will match the ITStatus type
    if ((__HAL_GPIO_EXTI_GET_IT(TS_INT_PIN) >> 13) == SET)
    {
        //TODO avoid double tapping
        BSP_TS_GetState(&TS_State);
        em_arg_t out;
        memset(&out, 0, sizeof(out));

        point_t point = {.x = TS_State.touchX[0], .y = TS_State.touchY[0]};

        out.size = sizeof(point);
        memcpy(out.data, &point, out.size);
        em_emit(EM_EVENT_PRESS, &out);

        __HAL_GPIO_EXTI_CLEAR_IT(TS_INT_PIN);
    }
}
