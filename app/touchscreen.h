#ifndef TOUCHSCREEN_H_
#define TOUCHSCREEN_H_

#include "stm32f7xx.h"
#include "stm32746g_discovery_ts.h"
#include "event_manager.h"

void ts_init(void);
void ts_press_handler(const em_arg_t *in); //EM_EVENT_PRESS

#endif /*TOUCHSCREEN_H_*/
