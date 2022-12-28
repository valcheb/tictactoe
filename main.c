#include "mcu.h"
#include "display.h"
#include "touchscreen.h"
#include "game_screen.h"
#include "title_screen.h"
#include "event_manager.h"
#include "ttt.h"

void screen_title(const em_arg_t *in)
{
    em_unsubscribe(EM_EVENT_PRESS, gs_press_control);
    em_subscribe(EM_EVENT_PRESS, title_press_control);

    title_greetings_view(NULL); //tictactoe, local button, net button
}

void screen_game(const em_arg_t *in) //maybe in will contain first player
{
    em_unsubscribe(EM_EVENT_PRESS, title_press_control);
    em_subscribe(EM_EVENT_PRESS, gs_press_control);

    gs_init_view();
}

void local_game(const em_arg_t *in)
{
    //ttt_local_init();
    ttt_init(NULL);
    //em_unsubscribe(EM_EVENT_PLAY_AGAIN_PRESSED, net_game);
    em_subscribe(EM_EVENT_PLAY_AGAIN_PRESSED, local_game);
    em_subscribe(EM_EVENT_CELL_CALCULATED, ttt_handler);
    screen_game(NULL);
}

void net_game(const em_arg_t *in)
{
    //ttt_net_init();
    //em_unsubscribe(PLAY_AGAIN, local_game);
    //em_subscribe(PLAY_AGAIN, net_game);
    screen_game(NULL);
}

void on_boot(void)
{
    //init view and control elements
    title_screen_init();
    game_screen_init();

    //subscribe common events
    em_subscribe(EM_EVENT_LOCAL_PRESSED, local_game);
    //em_subscribe(EM_EVENT_NET_PRESSED, net_game);
    em_subscribe(EM_EVENT_NEW_TURN, gs_player_view);
    em_subscribe(EM_EVENT_PLAYER_TURN, gs_turn_view);
    em_subscribe(EM_EVENT_END_OF_GAME, gs_winner_view);
    em_subscribe(EM_EVENT_MAIN_MENU_PRESSED, screen_title);
    em_subscribe(EM_EVENT_BOOTED, screen_title);

    em_emit(EM_EVENT_BOOTED, NULL);
}

int main(void)
{
    mcu_init();
    disp_init();
    ts_init();
    em_init();

    on_boot();

    while(1)
    {
        em_handler();
    }

    return 0;
}
