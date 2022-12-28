#include "display.h"
#include <string.h>

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
}

void disp_clear(void)
{
    BSP_LCD_Clear(BSP_LCD_GetBackColor());
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

void disp_cross(point_t point)
{
    uint8_t cell_half = cm_get_cell_length() / 2;

    BSP_LCD_DrawLine(point.x - cell_half,
                     point.y - cell_half,
                     point.x + cell_half,
                     point.y + cell_half);
    BSP_LCD_DrawLine(point.x + cell_half,
                     point.y - cell_half,
                     point.x - cell_half,
                     point.y + cell_half);
}

void disp_nought(point_t point)
{
    BSP_LCD_DrawCircle(point.x, point.y, cm_get_cell_length() / 2);
}

void disp_button(const button_t *button)
{
    BSP_LCD_DisplayStringAt(button_get_pos(button).x + 5, button_get_pos(button).y + 5, (uint8_t*)button->label, LEFT_MODE);
    BSP_LCD_DrawRect(button_get_pos(button).x,
                     button_get_pos(button).y,
                     button_get_hsize(button),
                     button_get_vsize(button));
}
