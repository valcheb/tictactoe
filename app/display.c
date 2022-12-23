#include "display.h"
#include <stdlib.h>
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
    cm_set_cell_length(86);
    cm_set_field_start(10, 10);
    cm_set_player_turn_pos(10 + 86 * 3 + 5, 10);
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

void disp_player(const em_arg_t *in)
{
    player_e player;
    char player_str[25];
    memset(player_str, 0, sizeof(player_str));
    char pl_char[2];
    point_t pl_pos = cm_get_player_turn_pos();
    sFONT *font    = BSP_LCD_GetFont();

    if (in == NULL)
    {
        return ;
    }
    memcpy(&player, in->data, in->size);

    itoa(player, pl_char, 10);

    strcat(player_str, "Player ");
    strcat(player_str, pl_char);
    strcat(player_str, "`s");

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(pl_pos.x, pl_pos.y, (uint8_t*)player_str, LEFT_MODE);
    BSP_LCD_DisplayStringAt(pl_pos.x, pl_pos.y + font->Height, (uint8_t*)"turn to play", LEFT_MODE);
}

void disp_turn(const em_arg_t *in) //EM_EVENT_PLAYER_TURN
{
    turn_t turn;
    if (in == NULL)
    {
        return ;
    }
    memcpy(&turn, in->data, in->size);

    if (turn.player == PLAYER_1)
    {
        disp_cross(cm_cell_to_point(turn.cell));
    }
    else if (turn.player == PLAYER_2)
    {
        disp_nought(cm_cell_to_point(turn.cell));
    }
}
