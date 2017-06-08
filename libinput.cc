#include <stdio.h>
#include <fcntl.h>

#include <linux/input.h>

#include "libinput.h"

#define EVENT_PATH "/dev/input/event0"

#define MAX_PATH 50

/**
 * @brief Open file stream
 * @param fl  double pointer to file stream
 * @return status
 */
int16_t input_open(FILE **fl)
{
  // Open stream
  *fl = fopen(EVENT_PATH, "rb");
  if (fl == NULL)
  {
    perror("fopen error\n");

    return -1;
  }

  return 0;
}

/**
 * @brief Close file stream
 * @param fl  pointer to file stream
 * @return status
 */
int16_t input_close(FILE* fl)
{
  fclose(fl);

  return 0;
}

/**
 * @brief Read file stream
 * @param fl  pointer to file stream
 * @return status
 */
int16_t input_read(FILE* fl)
{
  char data;

  fread(&data, sizeof(data), 1, fl);

  printf("data = %X\n", data);

  //printf("time %u; type =  %X; code = %X; value = %X\n",
  //    ev.time, ev.type, ev.code, ev.value);

  return 0;
}
