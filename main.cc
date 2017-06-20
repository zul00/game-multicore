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

void *poll_input(void *arg)
{
  FILE **fl;
  struct input_event inp;

  /* Initialize */
  // Allocate pointer to pointer of file
  fl = (FILE**)malloc(sizeof(FILE*));
  // Open stream
  input_open(fl);

  // Check FIFO
  wr_input->validate();

  for (;;)
  {
    inp = input_read(*fl);

    printf("size = %lu; ", sizeof(inp.time));
    printf("type =  %X; code = %X; value = %lu\n",
        inp.type, inp.code, inp.value);

    if (inp.type == 1)
    {
      wr_input->push(inp.code);
    }
  }

  // Close stream
  input_close(*fl);

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
  //pid_t pid[N_CORE];
  int16_t event = 0;

  printf("Hello Game!!!\n");

  /* Initialize */
  


  while(1)
  {
    event = buttons_event();

    printf("Event = 0x%02X\n", event);

    switch(event)
    {
      case 1:
        if (button_pressed(0) == true)
          printf("Button 1 Pressed\n");
        else if (button_released(0) == true)
          printf("Button 1 Released\n");
        else
          printf("No event on 1\n");

        break;

      case 2:
        if (button_pressed(1) == true)
          printf("Button 2 Pressed\n");
        else if (button_released(1) == true)
          printf("Button 2 Released\n");
        else
          printf("No event on 2\n");

        break;

      case 4:
        if (button_pressed(2) == true)
          printf("Button 4 Pressed\n");
        else if (button_released(2) == true)
          printf("Button 4 Released\n");
        else
          printf("No event on 4\n");

        break;

      case 8:
        if (button_pressed(3) == true)
          printf("Button 8 Pressed\n");
        else if (button_released(3) == true)
          printf("Button 8 Released\n");
        else
          printf("No event on 8\n");

        break;

      case 16:
        if (button_pressed(4) == true)
          printf("Button 16 Pressed\n");
        else if (button_released(4) == true)
          printf("Button 16 Released\n");
        else
          printf("No event on 16\n");

        break;

      default:
        printf("Unknown event\n");
        break;
    };

//    switch(event) {
//      case 1:
//
//      case 2:
//      case 4:
//    }



    //printf("Buttons = 0x%02X; event = 0x%02X\n", stat, event);

    usleep(500000);
  }
  
  
  
  
  
//  CFifoPtr<uint16_t> ff_input = CFifo<uint16_t>::Create(1, wr_input, 2, rd_input, 10);
//  if(!ff_input.valid()) ERREXIT("Error creating buffer");
//
//  // Create Process
//  if(int e=CreateProcess(pid[0], poll_input, NULL, PROC_DEFAULT_TIMESLICE,
//        PROC_DEFAULT_STACK, 1))
//    ERREXIT2("Process creation failed: %i", e);
//  if(int e=CreateProcess(pid[1], display, NULL, PROC_DEFAULT_TIMESLICE,
//        PROC_DEFAULT_STACK, 2))
//    ERREXIT2("Process creation failed: %i", e);
//
//  // Set Process Flag
//  if(int e=SetProcessFlags(pid[0], PROC_FLAG_JOINABLE, 1))
//    ERREXIT2("While setting process flags: %i", e);
//  if(int e=SetProcessFlags(pid[1], PROC_FLAG_JOINABLE, 2))
//    ERREXIT2("While setting process flags: %i", e);
//
//  // Start process
//  if(int e=StartProcess(pid[0], 1)) 
//    ERREXIT2("Could not start process: %i", e);
//  if(int e=StartProcess(pid[1], 2)) 
//    ERREXIT2("Could not start process: %i", e);
//
//  if(int e=WaitProcess(pid[0], NULL, 1)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[0], 1, e);
//  if(int e=WaitProcess(pid[1], NULL, 2)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[1], 2, e);

  printf("Game Over\n");

  return 0;
}
