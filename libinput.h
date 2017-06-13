#ifndef __LIBINPUT_H
#define __LIBINTPUT_H

#include <stdint.h>
#include <helix.h>

struct input_event
{
  struct timeval time; 

  uint16_t type;
  uint16_t code;
  int32_t value;
};

int16_t input_open(FILE **fl);
struct input_event input_read(FILE *fl);
int16_t input_close(FILE *fl);

#endif
