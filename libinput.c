#include <stdio.h>
#include <fcntl.h>
#include "libinput.h"

#define EVENT_PATH "/dev/input/event0"

#define MAX_PATH 50

int16_t input_open(void)
{
  char path[MAX_PATH];
  FILE *fl = NULL;
  char data;

  // Open stream
  fl = fopen(EVENT_PATH, "rb");
  if (fl == NULL)
  {
    perror("fopen error\n");

    return -1;
  }

  // Read stream
  while (1)
  {
    fread(&data, sizeof(data), 1, fl);

    printf("input = %X\n", data);
  }

  // Close stream
  fclose(fl);

  return 0;
}
