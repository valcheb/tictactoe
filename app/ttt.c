#include "ttt.h"
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

static struct
{
    player_e curr_player;
    uint8_t  cells[CELL_AMOUNT];
    uint8_t  turn_number;
} ttt_ctx;

static bool ttt_set_cell(uint8_t cell)
{
    if (ttt_ctx.cells[cell] != PLAYER_EMPTY) //cell already drawn
    {
        return false;
    }

    ttt_ctx.cells[cell] = ttt_ctx.curr_player;
    ttt_ctx.turn_number++;

    return true;
}

static void ttt_change_player(player_e *player)
{
    if (*player == PLAYER_1)
    {
        *player = PLAYER_2;
    }
    else if (*player == PLAYER_2)
    {
        *player = PLAYER_1;
    }
}

void ttt_init(const em_arg_t *in)
{
    em_arg_t out;
    ttt_ctx.curr_player = PLAYER_1;
    memset(ttt_ctx.cells, PLAYER_EMPTY, CELL_AMOUNT);
    ttt_ctx.turn_number = 0;

    out.size = sizeof(ttt_ctx.curr_player);
    memcpy(out.data, &ttt_ctx.curr_player, out.size);
    em_emit(EM_EVENT_NEW_TURN, &out);
}

void ttt_handler(const em_arg_t *in) //EM_EVENT_CELL_CALCULATED
{
    uint8_t    cell = 0;
    em_arg_t   out;
    turn_t turn;

    if (in == NULL)
    {
        return ;
    }
    memcpy(&cell, in->data, in->size);

    if (!ttt_set_cell(cell))
    {
        return ;
    }

    turn.player = ttt_ctx.curr_player;
    turn.cell   = cell;

    out.size = sizeof(turn);
    memcpy(out.data, &turn, out.size);
    em_emit(EM_EVENT_PLAYER_TURN, &out);

    ttt_change_player(&ttt_ctx.curr_player);

    memset(&out, 0, sizeof(out));
    out.size = sizeof(ttt_ctx.curr_player);
    memcpy(out.data, &ttt_ctx.curr_player, out.size);
    em_emit(EM_EVENT_NEW_TURN, &out);
}
