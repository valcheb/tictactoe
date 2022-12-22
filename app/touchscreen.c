#include "touchscreen.h"

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
        if (TS_State.touchDetected)
        {
            //do something
        }
        __HAL_GPIO_EXTI_CLEAR_IT(TS_INT_PIN);
    }
}
