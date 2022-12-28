#include "button.h"
#include <string.h>

void button_init(button_t *button, const point_t *pos, uint16_t hsize, uint16_t vsize, const char *label, uint16_t lsize)
{
    if (lsize <= LABEL_SIZE)
    {
        memcpy(button->label, label, lsize);
    }
    button_set_pos(button, *pos);
    button_set_hsize(button, hsize);
    button_set_vsize(button, vsize);
    button_hide(button);
}

bool button_is_pressed(const button_t *button, point_t press)
{
    bool hrange = press.x >= button->left_top.x && press.x <= button->left_top.x + button->hsize;
    bool vrange = press.y >= button->left_top.y && press.y <= button->left_top.y + button->vsize;

    if (hrange && vrange)
    {
        return true;
    }

    return false;
}

void button_set_pos(button_t *button, point_t left_top)
{
    button->left_top = left_top;
}

point_t button_get_pos(const button_t *button)
{
    return button->left_top;
}

void button_set_hsize(button_t *button, uint16_t hsize)
{
    button->hsize = hsize;
}

uint16_t button_get_hsize(const button_t *button)
{
    return button->hsize;
}

void button_set_vsize(button_t *button, uint16_t vsize)
{
    button->vsize = vsize;
}

uint16_t button_get_vsize(const button_t *button)
{
    return button->vsize;
}

void button_hide(button_t *button)
{
    button->hidden = true;
}

void button_show(button_t *button)
{
    button->hidden = false;
}

bool button_is_hidden(const button_t *button)
{
    return button->hidden;
}
