#ifndef GAME_SCREEN_H_
#define GAME_SCREEN_H_

#include "common.h"
#include "event_manager.h"

void game_screen_init(void);
void gs_new_game_view(void);
void gs_press_control(const em_arg_t *in); //EM_EVENT_PRESS
void gs_player_view(const em_arg_t *in); //EM_EVENT_NEW_TURN
void gs_turn_view(const em_arg_t *in); //EM_EVENT_PLAYER_TURN
void gs_winner_view(const em_arg_t *in); //EM_EVENT_END_OF_GAME

#endif /*GAME_SCREEN_H_*/
