/**
 * @file  main.c
 * @brief Multicore game 
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "libinput.h"

int main()
{
  FILE **fl;
  uint32_t ctr = 0;

  // Allocate pointer to pointer of file
  fl = (FILE**)malloc(sizeof(FILE*));

  printf("Hello Game!!!\n");

  // Open stream
  input_open(fl);

  // Read stream
  while (1)
  {
    printf("%lu; ", ctr);
    input_read(*fl);

    ctr++;
  }

  // Close stream
  input_close(*fl);

  printf("Game Over\n");

  return 0;
}
