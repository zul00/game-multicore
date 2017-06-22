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


enum btn_event
{
  BTN_ENTER = 0,
  BTN_RIGHT,
  BTN_DOWN,
  BTN_LEFT,
  BTN_UP,
  BTN_NO_EVENT
};

char const *btn_name[] =
{
  "ENTER",
  "RIGHT",
  "DOWN",
  "LEFT",
  "UP",
  "NO_EVENT"
};

CFifo<btn_event,CFifo<>::w> *wr_btn;
CFifo<btn_event,CFifo<>::r> *rd_btn;


/**
 * @brief Process to handle input
 *
 */
void *prc_input(void *arg)
{
  int16_t state = 0;
  uint8_t idx = 0;

  printf("Hello Poll!!!\n");

  // Check FIFO
  wr_btn->validate();
  while(1)
  {
    // Get state
    state = buttons_state();

    // Parse state
    for (idx=0; idx<5; idx++)
    {
      if (state & (1 << idx))
      {
        wr_btn->push((btn_event)idx);
      }
    }

    usleep(50000);
  }

  return NULL;
}

/**
 * @brief Process to handle player algorithm
 */
void *prc_player_alg(void *arg)
{
  uint16_t input = 0;

  /* Initialize */
  // Check FIFO
  rd_btn->validate();

  for (;;)
  {
    input = rd_btn->front();
    rd_btn->pop();

    if (input != BTN_NO_EVENT)
    {
      printf("button %u pressed = %s\n ", input, btn_name[input]);
    }
  }

  return NULL;
}

int main()
{
  pid_t pid[N_CORE];

  printf("Hello Game!!!\n");

  /* Initialize */
  CFifoPtr<btn_event> ff_input = CFifo<btn_event>::Create(1, wr_btn, 2, rd_btn, 10);
  if(!ff_input.valid()) ERREXIT("Error creating buffer");

  // Create Process
  if(int e=CreateProcess(pid[0], prc_input, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 1))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[1], prc_player_alg, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 2))
    ERREXIT2("Process creation failed: %i", e);

  // Set Process Flag
  if(int e=SetProcessFlags(pid[0], PROC_FLAG_JOINABLE, 1))
    ERREXIT2("While setting process flags: %i", e);
  if(int e=SetProcessFlags(pid[1], PROC_FLAG_JOINABLE, 2))
    ERREXIT2("While setting process flags: %i", e);

  // Start process
  if(int e=StartProcess(pid[0], 1)) 
    ERREXIT2("Could not start process: %i", e);
  if(int e=StartProcess(pid[1], 2)) 
    ERREXIT2("Could not start process: %i", e);

  if(int e=WaitProcess(pid[0], NULL, 1)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[0], 1, e);
  if(int e=WaitProcess(pid[1], NULL, 2)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[1], 2, e);

  printf("Game Over\n");

  return 0;
}
