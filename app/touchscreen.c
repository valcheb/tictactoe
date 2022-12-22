#include "touchscreen.h"
#include "display.h"

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
        //do something
        BSP_TS_GetState(&TS_State);
        if (TS_State.touchDetected)
        {
            BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
            char str[13];
            memset(str, 0, 13);
            strcat(str, "X = ");
            char ch[2];
            utoa(TS_State.touchX[0], ch, 10);
            strcat(str, ch);
            strcat(str, ", Y = ");
            utoa(TS_State.touchY[0], ch, 10);
            strcat(str, ch);
            BSP_LCD_ClearStringLine(3);
            BSP_LCD_DisplayStringAtLine(3, (uint8_t*)str);
        }
        __HAL_GPIO_EXTI_CLEAR_IT(TS_INT_PIN);
    }
}
