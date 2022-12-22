#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "stm32f7xx.h"
#include "stm32746g_discovery_lcd.h"
#include "common.h"

//TODO doc
void disp_init(void);
void disp_clean_field(void);
void disp_cross(point_t point);
void disp_nought(point_t point);

#endif /*DISPLAY_H_*/
