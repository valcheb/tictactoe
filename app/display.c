#include "display.h"
#include "common.h"

static void disp_lcd_init(void)
{
    /* LCD Initialization */
    BSP_LCD_Init();

    /* LCD Initialization */
    BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);

    /* Enable the LCD */
    BSP_LCD_DisplayOn();

    /* Select the LCD Background Layer  */
    BSP_LCD_SelectLayer(0);

    /* Clear the Background Layer */
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    /* Configure the transparency for foreground and background :
       Increase the transparency */
    BSP_LCD_SetTransparency(0, 255);
}

void disp_init(void)
{
    disp_lcd_init();
    cm_set_cell_length(85);
    cm_set_field_start(10, 10);
}

void disp_clean_field(void)
{
    point_t start       = cm_get_field_start();
    uint8_t cell_length = cm_get_cell_length();

    BSP_LCD_DrawHLine(start.x, start.y,                   3 * cell_length);
    BSP_LCD_DrawHLine(start.x, start.y + cell_length,     3 * cell_length);
    BSP_LCD_DrawHLine(start.x, start.y + 2 * cell_length, 3 * cell_length);
    BSP_LCD_DrawHLine(start.x, start.y + 3 * cell_length, 3 * cell_length);

    BSP_LCD_DrawVLine(start.x,                   start.x, 3 * cell_length);
    BSP_LCD_DrawVLine(start.x + cell_length,     start.x, 3 * cell_length);
    BSP_LCD_DrawVLine(start.x + 2 * cell_length, start.x, 3 * cell_length);
    BSP_LCD_DrawVLine(start.x + 3 * cell_length, start.x, 3 * cell_length);
}
