#include "mcu.h"
#include "display.h"
#include "touchscreen.h"
#include "event_manager.h"
#include "ttt.h"

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
    disp_init();
    ts_init();
    em_init();

    em_subscribe(EM_EVENT_PRESS, ts_press_handler);
    em_subscribe(EM_EVENT_CELL_CALCULATED, ttt_handler);
    em_subscribe(EM_EVENT_PLAYER_TURN, disp_turn);
    em_subscribe(EM_EVENT_INIT, ttt_init);
    em_subscribe(EM_EVENT_NEW_TURN, disp_player);

    disp_clean_field();
    em_emit(EM_EVENT_INIT, NULL);

    while(1)
    {
        em_handler();
    }

    /*disp_clean_field();

    player_e cells[9] =
    {
        PLAYER_1, PLAYER_2, PLAYER_2,
        PLAYER_2, PLAYER_1, PLAYER_1,
        PLAYER_1, PLAYER_2, PLAYER_1
    };

    for (int i = 0; i < 9; i++)
    {
        disp_turn(cells[i], i);
    }*/

    /*setup_led();
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
    }*/

    /* NotReached */
    return 0;
}
