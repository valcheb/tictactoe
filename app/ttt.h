#ifndef TTT_H_
#define TTT_H_

#include "event_manager.h"
#include "common.h"

void ttt_init(const em_arg_t *in); //EM_EVENT_INIT
void ttt_handler(const em_arg_t *in); //EM_EVENT_CELL_CALCULATED

#endif /*TTT_H_*/
