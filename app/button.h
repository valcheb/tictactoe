#ifndef BUTTON_H_
#define BUTTON_H_

#include "common.h"
#include <stdbool.h>

#define LABEL_SIZE 30

typedef struct
{
    char     label[LABEL_SIZE];
    point_t  left_top;
    uint16_t hsize;
    uint16_t vsize;
    bool     hidden;
} button_t;

void button_init(button_t *button, const point_t *pos, uint16_t hsize, uint16_t vsize, const char *label, uint16_t lsize);
bool button_is_pressed(const button_t *button, point_t press);
void button_set_pos(button_t *button, point_t left_top);
point_t button_get_pos(const button_t *button);
void button_set_hsize(button_t *button, uint16_t hsize);
uint16_t button_get_hsize(const button_t *button);
void button_set_vsize(button_t *button, uint16_t vsize);
uint16_t button_get_vsize(const button_t *button);
void button_hide(button_t *button);
void button_show(button_t *button);
bool button_is_hidden(const button_t *button);

#endif /*BUTTON_H_*/
