/**
 * @file  main.c
 * @brief Multicore game 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <helix.h>

#include "libinput.h"

#define ERREXIT(str) {fprintf(stderr, "Error: " str "\n"); exit(1);}
#define ERREXIT2(str, ...) {fprintf(stderr, "Error: " str "\n", __VA_ARGS__); exit(1);}


CFifo<uint16_t,CFifo<>::w> *wr_input;
CFifo<uint16_t,CFifo<>::r> *rd_input;

void *poll_input(void *arg)
{
  /* Initialize */
  // Allocate pointer to pointer of file
  fl = (FILE**)malloc(sizeof(FILE*));
  // Open stream
  input_open(fl);

  for (;;)
  {
    printf("%lu; ", ctr);
    input_read(*fl);

    ctr++;
  }

  // Close stream
  input_close(*fl);
}

int main()
{
  pid_t pid[N_CORE];
  FILE **fl;
  uint32_t ctr = 0;

  printf("Hello Game!!!\n");

  /* Initialize */
  // Prepare FIFO
  CFifoPtr<uint16_t> ff_input = CFifo<uint16_t>::Create(1, wr_input, 2, rd_input, 2);
  if(!fifo_input.valid()) ERREXIT("Error creating buffer");

  // Create Process
  if(int e=CreateProcess(pid[0], poll_input, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 1))
    ERREXIT2("Process creation failed: %i", e);

  // Set Process Flag
  if(int e=SetProcessFlags(pid[0], PROC_FLAG_JOINABLE, 1))
    ERREXIT2("While setting process flags: %i", e);

  // Start process
  if(int e=StartProcess(pid[0], 1)) 
    ERREXIT2("Could not start process: %i", e);

  if(int e=WaitProcess(pid[0], NULL, 1)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[0], 1, e);

  printf("Game Over\n");

  return 0;
}
