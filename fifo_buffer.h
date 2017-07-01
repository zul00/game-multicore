#ifndef __FIFO_BUFFER_H
#define __FIFO_BUFFER_H

#include <helix.h>

extern CFifo<btn_event_e,CFifo<>::w> *wr_btn;
extern CFifo<btn_event_e,CFifo<>::r> *rd_btn;

#endif
