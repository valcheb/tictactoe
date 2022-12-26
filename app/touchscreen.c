#include "touchscreen.h"
#include "common.h"
#include <string.h>
#include "event_manager.h"
#include "stm32f7xx.h"
#include "stm32746g_discovery_ts.h"

static TS_StateTypeDef TS_State;

void ts_init(void)
{
    BSP_TS_Init(FT5336_MAX_WIDTH, FT5336_MAX_HEIGHT);
    BSP_TS_ITConfig();
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
