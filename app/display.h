#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "stm32f7xx.h"
#include "stm32746g_discovery_lcd.h"
#include "common.h"
#include "event_manager.h"

//TODO doc
void disp_init(void);
void disp_clean_field(void);
void disp_cross(point_t point);
void disp_nought(point_t point);
void disp_player(const em_arg_t *in); //EM_EVENT_NEW_TURN
void disp_turn(const em_arg_t *in); //EM_EVENT_PLAYER_TURN
void disp_winner(const em_arg_t *in); //EM_EVENT_END_OF_GAME

#endif /*DISPLAY_H_*/
