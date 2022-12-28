#include "touchscreen.h"
#include "common.h"
#include <string.h>
#include "event_manager.h"
#include "stm32f7xx.h"
#include "stm32746g_discovery_ts.h"

#define DOUBLE_TAP_PAUSE 100

static TS_StateTypeDef TS_State;

void ts_init(void)
{
    BSP_TS_Init(FT5336_MAX_WIDTH, FT5336_MAX_HEIGHT);
    BSP_TS_ITConfig();
}

void EXTI15_10_IRQHandler(void)
{
    static uint32_t tickstart = 0;

    //__HAL_GPIO_EXTI_GET_IT returns PR register value by mask TS_INT_PIN
    //this value, shifted to the right by a number of bits equal to the exti line number,
    //will match the ITStatus type
    if ((__HAL_GPIO_EXTI_GET_IT(TS_INT_PIN) >> 13) == SET)
    {
        BSP_TS_GetState(&TS_State);
        if (TS_State.touchDetected && (HAL_GetTick() - tickstart >= DOUBLE_TAP_PAUSE))
        {
            em_arg_t out;
            point_t  point = {.x = TS_State.touchX[0], .y = TS_State.touchY[0]};

            out.size = sizeof(point);
            memcpy(out.data, &point, out.size);
            em_emit(EM_EVENT_PRESS, &out);

            tickstart = HAL_GetTick();
        }
        __HAL_GPIO_EXTI_CLEAR_IT(TS_INT_PIN);
    }
}
