#include "game_screen.h"
#include "display.h"
#include <string.h>
#include <stdio.h>

void gs_player_view(const em_arg_t *in)
{
    player_e player;
    char player_str[11];
    memset(player_str, 0, sizeof(player_str));
    char pl_char[2];
    point_t pl_pos = cm_get_player_turn_pos();
    sFONT *font    = BSP_LCD_GetFont();

    if (in == NULL)
    {
        return ;
    }
    memcpy(&player, in->data, in->size);

    sprintf(pl_char, "%d", (uint8_t)player);

    strcat(player_str, "Player ");
    strcat(player_str, pl_char);
    strcat(player_str, "`s");

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(pl_pos.x, pl_pos.y, (uint8_t*)player_str, LEFT_MODE);
    BSP_LCD_DisplayStringAt(pl_pos.x, pl_pos.y + font->Height, (uint8_t*)"turn to play", LEFT_MODE);
}

void gs_turn_view(const em_arg_t *in) //EM_EVENT_PLAYER_TURN
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

void gs_winner_view(const em_arg_t *in) //EM_EVENT_END_OF_GAME
{
    player_e player;
    point_t pl_pos = cm_get_player_turn_pos();
    sFONT *font    = BSP_LCD_GetFont();
    char player_str[9];
    memset(player_str, 0, sizeof(player_str));
    char pl_char[2];

    if (in == NULL)
    {
        return ;
    }
    memcpy(&player, in->data, in->size);

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    //clean
    BSP_LCD_DisplayStringAt(pl_pos.x, pl_pos.y, (uint8_t*)"          ", LEFT_MODE);
    BSP_LCD_DisplayStringAt(pl_pos.x, pl_pos.y + font->Height, (uint8_t*)"            ", LEFT_MODE);

    if (player)
    {
        sprintf(pl_char, "%d", (uint8_t)player);
        strcat(player_str, "Player ");
        strcat(player_str, pl_char);
        BSP_LCD_DisplayStringAt(pl_pos.x, pl_pos.y, (uint8_t*)player_str, LEFT_MODE);
        BSP_LCD_DisplayStringAt(pl_pos.x, pl_pos.y + font->Height, (uint8_t*)"wins!", LEFT_MODE);
    }
    else
    {
        BSP_LCD_DisplayStringAt(pl_pos.x, pl_pos.y, (uint8_t*)"Draw!", LEFT_MODE);
    }
}
