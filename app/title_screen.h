#ifndef TITLE_SCREEN_H_
#define TITLE_SCREEN_H_

#include "event_manager.h"
#include "common.h"

//TODO doc
void title_screen_init(void);
void title_event_init(void);
void title_event_uninit(void);
void title_press_control(const em_arg_t *in); //EM_EVENT_PRESSED
void title_greetings_view(const em_arg_t *in); //EM_EVENT_INIT

#endif /*TITLE_SCREEN_H_*/
