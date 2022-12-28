#include "game_screen.h"
#include "display.h"
#include "button.h"
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

static button_t play_again;
static button_t main_menu;

static uint8_t gs_hit_cell(point_t point)
{
    bool hrange = false;
    bool vrange = false;
    uint8_t row = 0;
    uint8_t col = 0;
    point_t start       = cm_get_field_start();
    uint8_t cell_length = cm_get_cell_length();

    for (int i = 0; i < CELL_AMOUNT; i++)
    {
        row = i / 3;
        col = i - row * 3;

        hrange = point.x > start.x + col * cell_length && point.x < start.x + (col + 1) * cell_length;
        vrange = point.y > start.y + row * cell_length && point.y < start.y + (row + 1) * cell_length;

        if (hrange && vrange) //press point belongs to cell
        {
            return i;
        }
    }

    return CELL_AMOUNT;
}

void game_screen_init(void)
{
    //hard code game screen view and control elements
    cm_set_cell_length(86);
    cm_set_field_start(10, 10);
    cm_set_player_turn_pos(10 + 86 * 3 + 5, 10);

    point_t pos;
    pos.x = 280;
    pos.y = 75;
    char play_again_label[] = "Play again";
    button_init(&play_again, &pos, 180, 33, play_again_label, sizeof(play_again_label));

    pos.x = 280;
    pos.y = 130;
    char main_menu_label[] = "Main menu";
    button_init(&main_menu, &pos, 165, 33, main_menu_label, sizeof(main_menu_label));
}

void gs_press_control(const em_arg_t *in) //EM_EVENT_PRESS
{
    point_t  point;
    em_arg_t out;
    bool     hrange = false;
    bool     vrange = false;
    uint8_t  cell   = CELL_AMOUNT;
    point_t  start       = cm_get_field_start();
    uint8_t  cell_length = cm_get_cell_length();

    if (in == NULL)
    {
        return ;
    }
    memcpy(&point, in->data, in->size);

    hrange = point.x > start.x && point.x < start.x + 3 * cell_length;
    vrange = point.y > start.y && point.y < start.y + 3 * cell_length;
    if (hrange && vrange) //press point belongs to field
    {
        cell = gs_hit_cell(point);
        if (cell < CELL_AMOUNT)
        {
            out.size = sizeof(cell);
            memcpy(out.data, &cell, out.size);
            em_emit(EM_EVENT_CELL_CALCULATED, &out);
        }
    }
}

void gs_new_game_view(void)
{
    disp_clear();
    disp_clean_field();
    disp_button(&play_again);
    button_show(&play_again);

    disp_button(&main_menu);
    button_show(&main_menu);
}

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
