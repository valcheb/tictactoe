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

static bool ttt_end_of_game(player_e *player)
{
    if (ttt_ctx.turn_number < 5)
    {
        return false;
    }

    //check horizontal
    for (int i = 0; i <= 6; i += 3)
    {
        if (ttt_ctx.cells[i] != PLAYER_EMPTY         &&
            ttt_ctx.cells[i] == ttt_ctx.cells[i + 1] &&
            ttt_ctx.cells[i] == ttt_ctx.cells[i + 2])
        {
            *player = ttt_ctx.cells[i];
            return true;
        }
    }

    //check vertical
    for (int i = 0; i <= 2; i++)
    {
        if (ttt_ctx.cells[i] != PLAYER_EMPTY         &&
            ttt_ctx.cells[i] == ttt_ctx.cells[i + 3] &&
            ttt_ctx.cells[i] == ttt_ctx.cells[i + 6])
        {
            *player = ttt_ctx.cells[i];
            return true;
        }
    }

    //check diagonal
    if (ttt_ctx.cells[0] != PLAYER_EMPTY     &&
        ttt_ctx.cells[0] == ttt_ctx.cells[4] &&
        ttt_ctx.cells[0] == ttt_ctx.cells[8])
    {
        *player = ttt_ctx.cells[0];
        return true;
    }

    if (ttt_ctx.cells[2] != PLAYER_EMPTY     &&
        ttt_ctx.cells[2] == ttt_ctx.cells[4] &&
        ttt_ctx.cells[2] == ttt_ctx.cells[6])
    {
        *player = ttt_ctx.cells[2];
        return true;
    }

    //check draw
    if (ttt_ctx.turn_number == CELL_AMOUNT)
    {
        *player = PLAYER_EMPTY;
        return true;
    }

    return false;
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
    uint8_t  cell = 0;
    em_arg_t out;
    player_e winner;
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

    if (ttt_end_of_game(&winner))
    {
        out.size = sizeof(winner);
        memcpy(out.data, &winner, out.size);
        em_emit(EM_EVENT_END_OF_GAME, &out);
    }
    else
    {
        ttt_change_player(&ttt_ctx.curr_player);
        memset(&out, 0, sizeof(out));
        out.size = sizeof(ttt_ctx.curr_player);
        memcpy(out.data, &ttt_ctx.curr_player, out.size);
        em_emit(EM_EVENT_NEW_TURN, &out);
    }
}
