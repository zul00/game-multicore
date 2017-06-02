#ifndef __LIBINPUT_H
#define __LIBINTPUT_H

#include <stdint.h>

int16_t input_open(FILE **fl);
int16_t input_read(FILE *fl);
int16_t input_close(FILE *fl);

#endif
