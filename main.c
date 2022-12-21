#include "mcu.h"
#include "display.h"
#include "event_manager.h"

static void led_on(void)
{
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_SET);
}

static void led_off(void)
{
    HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_RESET);
}

static void setup_led(void)
{
    GPIO_InitTypeDef gpioInitStructure;

    __HAL_RCC_GPIOI_CLK_ENABLE();
    gpioInitStructure.Pin = GPIO_PIN_1;
    gpioInitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    gpioInitStructure.Pull = GPIO_PULLUP;
    gpioInitStructure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOI, &gpioInitStructure);
    led_off();
}

int main(void)
{
    uint32_t i = 0;

    mcu_init();
    disp_lcd_init();

    setup_led();
    while (1)
    {
        if (i % 2)
        {
            led_off();
            BSP_LCD_SetTextColor(LCD_COLOR_RED);
            BSP_LCD_DisplayStringAtLine(5, (uint8_t*)"Sample Text!");
        }
        else
        {
            led_on();
            BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
            BSP_LCD_DisplayStringAtLine(5, (uint8_t*)"Sample Text!");
        }

        HAL_Delay(1000);
        ++i;
    }

    /* NotReached */
    return 0;
}
