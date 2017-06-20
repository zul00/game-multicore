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

#define N_CORE 2


CFifo<uint16_t,CFifo<>::w> *wr_input;
CFifo<uint16_t,CFifo<>::r> *rd_input;

enum btn_stat_t
{
  BTN0 = 0,
  BTN1,
  BTN2,
  BTN3,
  BTN4
};

void *poll_input(void *arg)
{
  int16_t state = 0;
  uint8_t idx = 0;

  printf("Hello Poll!!!\n");

  while(1)
  {
    // Get state
    state = buttons_state();
    printf("Event = 0x%02X\n", state);

    // Parse state
    for (idx=0; idx<5; idx++)
    {
      if (state & (1 << idx))
      {
        printf("Button %u pressed\n", idx);
      }
    }

    usleep(50000);
  }

  return NULL;
}

void *display(void *arg)
{
  uint16_t input = 0;

  /* Initialize */
  // Check FIFO
  rd_input->validate();

  for (;;)
  {
    input = rd_input->front();
    rd_input->pop();

    printf("INP=%X\n ", input);
  }

  return NULL;
}

int main()
{
  pid_t pid[N_CORE];

  printf("Hello Game!!!\n");

  /* Initialize */
  CFifoPtr<uint16_t> ff_input = CFifo<uint16_t>::Create(1, wr_input, 2, rd_input, 10);
  if(!ff_input.valid()) ERREXIT("Error creating buffer");

  // Create Process
  if(int e=CreateProcess(pid[0], poll_input, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 1))
    ERREXIT2("Process creation failed: %i", e);
//  if(int e=CreateProcess(pid[1], display, NULL, PROC_DEFAULT_TIMESLICE,
//        PROC_DEFAULT_STACK, 2))
//    ERREXIT2("Process creation failed: %i", e);

  // Set Process Flag
  if(int e=SetProcessFlags(pid[0], PROC_FLAG_JOINABLE, 1))
    ERREXIT2("While setting process flags: %i", e);
//  if(int e=SetProcessFlags(pid[1], PROC_FLAG_JOINABLE, 2))
//    ERREXIT2("While setting process flags: %i", e);

  // Start process
  if(int e=StartProcess(pid[0], 1)) 
    ERREXIT2("Could not start process: %i", e);
//  if(int e=StartProcess(pid[1], 2)) 
//    ERREXIT2("Could not start process: %i", e);

  if(int e=WaitProcess(pid[0], NULL, 1)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[0], 1, e);
//  if(int e=WaitProcess(pid[1], NULL, 2)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[1], 2, e);

  printf("Game Over\n");

  return 0;
}
