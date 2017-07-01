#ifndef __CORE_INPUT_H
#define __CORE_INPUT_H

enum btn_event_e
{
  BTN_ENTER = 0,
  BTN_RIGHT,
  BTN_DOWN,
  BTN_LEFT,
  BTN_UP,
  BTN_NO_EVENT
};

extern char const *btn_string[];

void *core_input(void *arg);

#endif
