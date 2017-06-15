#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>

#include "libinput.h"

#define EVENT_PATH "/dev/input/event0"

#define MAX_PATH 50

struct input_event
{
  struct timeval time; 
  uint16_t type;
  uint16_t code;
  int32_t value;
};


/**
 * @brief Open file stream
 * @param fl  double pointer to file stream
 * @return status
 */
int16_t input_open(FILE **fl)
{
  // Open stream
  *fl = fopen(EVENT_PATH, "rb");

  int err = ferror(*fl);

  if (err)
  {
    printf("Error in fd\n");
  }


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
int16_t input_read(void)
{
  struct input_event *ev;
  size_t rdim = 0;
  FILE *fl;

  // Allocate memory for event
  ev = (struct input_event*) malloc(sizeof(struct input_event));
  if (ev == NULL)
  {
    perror("ev malloc failed\n");
  }

  // Open stream
  printf("Opening %s\n", EVENT_PATH);
  fl = fopen(EVENT_PATH, "rb");
  if (fl == NULL)
  {
    perror("fopen failed\n");
  }

  // Check pointer
  printf("ev, %lX\n", (uintptr_t)ev);
  printf("fl, %lX\n", (uintptr_t)fl);

  //fflush(stdout);
  //fflush(fl);
  rdim = fread(ev, sizeof(struct input_event), 1, fl);

  printf("dim %lu\n", rdim);

//
//  printf("size = %lu; ", sizeof(ev.time));
//  printf("type =  %X; code = %X; value = %lu\n",
//      ev.type, ev.code, ev.value);

  free(ev);
  fclose(fl);

  return 0;
}
