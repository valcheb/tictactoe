#include "stm32f7xx.h"
#include "stm32746g_discovery_lcd.h"

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

static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;
    HAL_StatusTypeDef ret = HAL_OK;

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 400;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 8;

    ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
    if (ret != HAL_OK)
    {
        while (1)
        {
            ;
        }
    }

    /* Activate the OverDrive to reach the 200 MHz Frequency */
    ret = HAL_PWREx_EnableOverDrive();
    if (ret != HAL_OK)
    {
        while (1)
        {
            ;
        }
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6);
    if (ret != HAL_OK)
    {
        while (1)
        {
            ;
        }
    }
}

#if 0
static void LCD_Config(void)
{
    /* LCD Initialization */
    BSP_LCD_Init();

    /* LCD Initialization */
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);
    BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS + (BSP_LCD_GetXSize() * BSP_LCD_GetYSize() * 4));

    /* Enable the LCD */
    BSP_LCD_DisplayOn();

    /* Select the LCD Background Layer  */
    BSP_LCD_SelectLayer(0);

    /* Clear the Background Layer */
    BSP_LCD_Clear(LCD_COLOR_BLACK);

    /* Select the LCD Foreground Layer  */
    BSP_LCD_SelectLayer(1);

    /* Clear the Foreground Layer */
    BSP_LCD_Clear(LCD_COLOR_BLACK);

    /* Configure the transparency for foreground and background :
       Increase the transparency */
    BSP_LCD_SetTransparency(0, 0);
    BSP_LCD_SetTransparency(1, 100);
}
#endif

int main(void)
{
    uint32_t i = 0;

    SystemClock_Config();
    //LCD_Config();

    setup_led();
    volatile uint32_t clock = HAL_RCC_GetSysClockFreq();

    while (1)
    {
        if (i % 2)
        {
            led_off();
        }
        else
        {
            led_on();
        }

        for (int j = 0; j < 1000000; j++)
        {
            __asm volatile("nop");
        }
        ++i;
    }

    /* NotReached */
    return 0;
}
