/**
 * @file  main.c
 * @brief Multicore game 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <helix.h>

#include "game_param.h"

#include "core_input.h"
#include "core_player.h"
#include "core_render.h"
#include "core_enemy.h"

#define ERREXIT(str) {fprintf(stderr, "Error: " str "\n"); exit(1);}
#define ERREXIT2(str, ...) {fprintf(stderr, "Error: " str "\n", __VA_ARGS__); exit(1);}

#define N_CORE 4

/* Declare FIFO buffer */
CFifo<btn_event_e,CFifo<>::w> *wr_btn;
CFifo<btn_event_e,CFifo<>::r> *rd_btn;

CFifo<player_param_t,CFifo<>::w> *wr_player;
CFifo<player_param_t,CFifo<>::r> *rd_player;

CFifo<bullet_param_t,CFifo<>::w> *wr_bullet;
CFifo<bullet_param_t,CFifo<>::r> *rd_bullet;

CFifo<bullet_param_t,CFifo<>::w> *wr_bullet2;
CFifo<bullet_param_t,CFifo<>::r> *rd_bullet2;


int main()
{
  pid_t pid[N_CORE];

  printf("Hello Game!!!\n");

  /* Initialize */
  CFifoPtr<btn_event_e> ff_input = CFifo<btn_event_e>::Create(1, wr_btn, 2, rd_btn, 10);
  if(!ff_input.valid()) ERREXIT("Error creating buffer");
  CFifoPtr<player_param_t> ff_player = CFifo<player_param_t>::Create(2, wr_player, 3, rd_player, 10);
  if(!ff_player.valid()) ERREXIT("Error creating buffer");
  CFifoPtr<bullet_param_t> ff_bullet = CFifo<bullet_param_t>::Create(2, wr_bullet, 3, rd_bullet, 10);
  if(!ff_bullet.valid()) ERREXIT("Error creating buffer");

  // Create Process
  if(int e=CreateProcess(pid[0], core_input, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 1))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[1], core_player, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 2))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[2], core_render, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 3))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[3], core_enemy, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 4))
    ERREXIT2("Process creation failed: %i", e);

  // Set Process Flag
  if(int e=SetProcessFlags(pid[0], PROC_FLAG_JOINABLE, 1))
    ERREXIT2("While setting process flags: %i", e);
  if(int e=SetProcessFlags(pid[1], PROC_FLAG_JOINABLE, 2))
    ERREXIT2("While setting process flags: %i", e);
  if(int e=SetProcessFlags(pid[2], PROC_FLAG_JOINABLE, 3))
    ERREXIT2("While setting process flags: %i", e);
  if(int e=SetProcessFlags(pid[3], PROC_FLAG_JOINABLE, 4))
    ERREXIT2("While setting process flags: %i", e);

  // Start process
  if(int e=StartProcess(pid[0], 1)) 
    ERREXIT2("Could not start process: %i", e);
  if(int e=StartProcess(pid[1], 2)) 
    ERREXIT2("Could not start process: %i", e);
  if(int e=StartProcess(pid[2], 3)) 
    ERREXIT2("Could not start process: %i", e);
  if(int e=StartProcess(pid[3], 4)) 
    ERREXIT2("Could not start process: %i", e);

  if(int e=WaitProcess(pid[0], NULL, 1)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[0], 1, e);
  if(int e=WaitProcess(pid[1], NULL, 2)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[1], 2, e);
  if(int e=WaitProcess(pid[2], NULL, 3)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[2], 3, e);
  if(int e=WaitProcess(pid[3], NULL, 4)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[3], 4, e);

  printf("Game Over\n");

  return 0;
}
