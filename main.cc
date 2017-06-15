/**
 * @file  main.c
 * @brief Multicore game 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <helix.h>

#include "libinput.h"

int main()
{
  FILE **fl;
  uint32_t ctr = 0;

  FlushDCache();

  // Allocate pointer to pointer of file
  fl = (FILE**)malloc(sizeof(FILE*));

  printf("Hello Game!!!\n");

  // Open stream
  //input_open(fl);
  printf("And here\n");

  // Read stream
  while (1)
  {
    printf("How about here\n");
    //printf("%lu; ", ctr);
    input_read();
    printf("here2\n");

    ctr++;
    sleep(1);
  }

  // Close stream
  input_close(*fl);

  printf("Game Over\n");

  return 0;
}
