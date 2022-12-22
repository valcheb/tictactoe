#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>

typedef struct
{
    uint8_t x;
    uint8_t y;
} point_t;

//TODO doc
void cm_set_cell_length(uint8_t length);
uint8_t cm_get_cell_length(void);
void cm_set_field_start(uint8_t x, uint8_t y);
point_t cm_get_field_start(void);

#endif /*COMMON_H_*/
