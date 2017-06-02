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

  // Allocate pointer to pointer of file
  fl = (FILE**)malloc(sizeof(FILE*));

  printf("Hello Game!!!\n");

  // Open stream
  input_open(fl);

  // Read stream
  while (1)
  {
    input_read(*fl);
  }

  // Close stream
  input_close(*fl);

  printf("Game Over\n");

  return 0;
}
