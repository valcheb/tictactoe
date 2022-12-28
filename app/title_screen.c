#include "title_screen.h"
#include "string.h"
#include "display.h"
#include "button.h"

static point_t  greetings;
static button_t local;
static button_t net;

void title_screen_init(void)
{
    //hard code title view and control elements
    greetings.x = 0;
    greetings.y = 40;
    point_t pos;

    pos.x = 148;
    pos.y = 105;
    char local_label[] = "Local game";
    button_init(&local, &pos, 180, 33, local_label, sizeof(local_label));

    pos.x = 165;
    pos.y = 180;
    char net_label[] = "Net game";
    button_init(&net, &pos, 145, 33, net_label, sizeof(net_label));
}

/*void title_event_init(void)
{
    em_subscribe(EM_EVENT_INIT, title_greetings_view);
    em_subscribe(EM_EVENT_PRESS, title_press_control);
}

void title_event_uninit(void)
{
    em_unsubscribe(EM_EVENT_INIT, title_greetings_view);
    em_unsubscribe(EM_EVENT_PRESS, title_press_control);
}*/

void title_greetings_view(const em_arg_t *in)
{
    disp_clear();

    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayStringAt(greetings.x, greetings.y, (uint8_t*)"Tic Tac Toe", CENTER_MODE);

    disp_button(&local);
    button_show(&local);
    //disp_button(&net);
    //button_show(&net);
}

void title_press_control(const em_arg_t *in) //EM_EVENT_PRESSED
{
    point_t  point;

    if (in == NULL)
    {
        return ;
    }
    memcpy(&point, in->data, in->size);

    if (!button_is_hidden(&local) && button_is_pressed(&local, point))
    {
        em_emit(EM_EVENT_LOCAL_PRESSED, NULL);
    }

    if (!button_is_hidden(&net) && button_is_pressed(&net, point))
    {
        em_emit(EM_EVENT_NET_PRESSED, NULL);
    }
}
