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
  printf("Hello Game!!!\n");

  input_open();

  printf("End of Game\n");

  return 0;
}
