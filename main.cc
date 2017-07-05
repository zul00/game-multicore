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
#include "core_collission.h"

#define ERREXIT(str) {fprintf(stderr, "Error: " str "\n"); exit(1);}
#define ERREXIT2(str, ...) {fprintf(stderr, "Error: " str "\n", __VA_ARGS__); exit(1);}

#define N_CORE 5

#define CORE_INPUT      1
#define CORE_PLAYER     2
#define CORE_RENDER     3
#define CORE_ENEMY      4
#define CORE_COLLISSION 5

/* Declare FIFO buffer */
CFifo<btn_event_e,CFifo<>::w> *wr_btn;
CFifo<btn_event_e,CFifo<>::r> *rd_btn;

CFifo<btn_event_e,CFifo<>::w> *wr_btne;
CFifo<btn_event_e,CFifo<>::r> *rd_btne;

CFifo<player_param_t,CFifo<>::w> *wr_player;
CFifo<player_param_t,CFifo<>::r> *rd_player;

CFifo<bool,CFifo<>::w> *wr_player_c;
CFifo<bool,CFifo<>::r> *rd_player_c;

CFifo<player_param_t,CFifo<>::w> *wr_player_r;
CFifo<player_param_t,CFifo<>::r> *rd_player_r;

CFifo<enemy_param_t,CFifo<>::w> *wr_enemy;
CFifo<enemy_param_t,CFifo<>::r> *rd_enemy;

CFifo<bool,CFifo<>::w> *wr_enemy_c;
CFifo<bool,CFifo<>::r> *rd_enemy_c;

CFifo<enemy_param_t,CFifo<>::w> *wr_enemy_r;
CFifo<enemy_param_t,CFifo<>::r> *rd_enemy_r;

CFifo<bullet_param_t,CFifo<>::w> *wr_bullet;
CFifo<bullet_param_t,CFifo<>::r> *rd_bullet;

CFifo<bool,CFifo<>::w> *wr_bullet_c;
CFifo<bool,CFifo<>::r> *rd_bullet_c;

CFifo<bullet_param_t,CFifo<>::w> *wr_bullet_r;
CFifo<bullet_param_t,CFifo<>::r> *rd_bullet_r;

CFifo<bullet_param_t,CFifo<>::w> *wr_ebullet;
CFifo<bullet_param_t,CFifo<>::r> *rd_ebullet;

CFifo<bool,CFifo<>::w> *wr_ebullet_c;
CFifo<bool,CFifo<>::r> *rd_ebullet_c;

CFifo<bullet_param_t,CFifo<>::w> *wr_ebullet_r;
CFifo<bullet_param_t,CFifo<>::r> *rd_ebullet_r;

int main()
{
  pid_t pid[N_CORE];

  printf("Hello Game!!!\n");

  /* Initialize */
  CFifoPtr<btn_event_e> ff_input = 
    CFifo<btn_event_e>::Create(CORE_INPUT, wr_btn, CORE_PLAYER, rd_btn, 10);
  if(!ff_input.valid()) ERREXIT("Error creating buffer");

  CFifoPtr<btn_event_e> ff_inpute = 
    CFifo<btn_event_e>::Create(CORE_INPUT, wr_btne, CORE_ENEMY, rd_btne, 10);
  if(!ff_inpute.valid()) ERREXIT("Error creating buffer");

  CFifoPtr<player_param_t> ff_player = 
    CFifo<player_param_t>::Create(CORE_PLAYER, wr_player, CORE_COLLISSION, rd_player, 10);
  if(!ff_player.valid()) ERREXIT("Error creating buffer");
  
  CFifoPtr<bool> ff_player_c = 
    CFifo<bool>::Create(CORE_COLLISSION, wr_player_c, CORE_PLAYER, rd_player_c, 10);
  if(!ff_player_c.valid()) ERREXIT("Error creating buffer");

  CFifoPtr<player_param_t> ff_player_r = 
    CFifo<player_param_t>::Create(CORE_PLAYER, wr_player_r, CORE_RENDER, rd_player_r, 10);
  if(!ff_player_r.valid()) ERREXIT("Error creating buffer");

  CFifoPtr<bullet_param_t> ff_bullet = 
    CFifo<bullet_param_t>::Create(CORE_PLAYER, wr_bullet, CORE_COLLISSION, rd_bullet, 10);
  if(!ff_bullet.valid()) ERREXIT("Error creating buffer");

  CFifoPtr<bool> ff_bullet_c = 
    CFifo<bool>::Create(CORE_COLLISSION, wr_bullet_c, CORE_PLAYER, rd_bullet_c, 10);
  if(!ff_bullet_c.valid()) ERREXIT("Error creating buffer");

  CFifoPtr<bullet_param_t> ff_bullet_r = 
    CFifo<bullet_param_t>::Create(CORE_PLAYER, wr_bullet_r, CORE_RENDER, rd_bullet_r, 10);
  if(!ff_bullet_r.valid()) ERREXIT("Error creating buffer");

  CFifoPtr<enemy_param_t> ff_enemy = 
    CFifo<enemy_param_t>::Create(CORE_ENEMY, wr_enemy, CORE_COLLISSION, rd_enemy, 10);
  if(!ff_enemy.valid()) ERREXIT("Error creating buffer");

  CFifoPtr<bool> ff_enemy_c = 
    CFifo<bool>::Create(CORE_COLLISSION, wr_enemy_c, CORE_ENEMY, rd_enemy_c, 10);
  if(!ff_enemy_c.valid()) ERREXIT("Error creating buffer");

  CFifoPtr<enemy_param_t> ff_enemy_r = 
    CFifo<enemy_param_t>::Create(CORE_ENEMY, wr_enemy_r, CORE_RENDER, rd_enemy_r, 10);
  if(!ff_enemy_r.valid()) ERREXIT("Error creating buffer");

  // Enemy bullet
  CFifoPtr<bullet_param_t> ff_ebullet = 
    CFifo<bullet_param_t>::Create(CORE_ENEMY, wr_ebullet, CORE_COLLISSION, rd_ebullet, 10);
  if(!ff_ebullet.valid()) ERREXIT("Error creating buffer");

  CFifoPtr<bool> ff_ebullet_c = 
    CFifo<bool>::Create(CORE_COLLISSION, wr_ebullet_c, CORE_ENEMY, rd_ebullet_c, 10);
  if(!ff_ebullet_c.valid()) ERREXIT("Error creating buffer");

  CFifoPtr<bullet_param_t> ff_ebullet_r = 
    CFifo<bullet_param_t>::Create(CORE_ENEMY, wr_ebullet_r, CORE_RENDER, rd_ebullet_r, 10);
  if(!ff_ebullet_r.valid()) ERREXIT("Error creating buffer");

  // Flush FIFO
  mc_flush(ff_input);
  mc_flush(ff_inpute);
  mc_flush(ff_player);
  mc_flush(ff_player_r);
  mc_flush(ff_enemy);
  mc_flush(ff_enemy_r);
  mc_flush(ff_enemy_c);
  mc_flush(ff_bullet);
  mc_flush(ff_bullet_r);
  mc_flush(ff_bullet_c);
  mc_flush(ff_ebullet);
  mc_flush(ff_ebullet_r);
  mc_flush(ff_ebullet_c);
  FlushDCache();

  // Create Process
  if(int e=CreateProcess(pid[0], core_input, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, CORE_INPUT))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[1], core_player, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, CORE_PLAYER))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[2], core_render, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, CORE_RENDER))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[3], core_enemy, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, CORE_ENEMY))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[4], core_collission, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, CORE_COLLISSION))
    ERREXIT2("Process creation failed: %i", e);

  // Set Process Flag
  if(int e=SetProcessFlags(pid[0], PROC_FLAG_JOINABLE, CORE_INPUT))
    ERREXIT2("While setting process flags: %i", e);
  if(int e=SetProcessFlags(pid[1], PROC_FLAG_JOINABLE, CORE_PLAYER))
    ERREXIT2("While setting process flags: %i", e);
  if(int e=SetProcessFlags(pid[2], PROC_FLAG_JOINABLE, CORE_RENDER))
    ERREXIT2("While setting process flags: %i", e);
  if(int e=SetProcessFlags(pid[3], PROC_FLAG_JOINABLE, CORE_ENEMY))
    ERREXIT2("While setting process flags: %i", e);
  if(int e=SetProcessFlags(pid[4], PROC_FLAG_JOINABLE, CORE_COLLISSION))
    ERREXIT2("While setting process flags: %i", e);

  // Start process
  if(int e=StartProcess(pid[0], CORE_INPUT)) 
    ERREXIT2("Could not start process: %i", e);
  if(int e=StartProcess(pid[1], CORE_PLAYER)) 
    ERREXIT2("Could not start process: %i", e);
  if(int e=StartProcess(pid[2], CORE_RENDER)) 
    ERREXIT2("Could not start process: %i", e);
  if(int e=StartProcess(pid[3], CORE_ENEMY)) 
    ERREXIT2("Could not start process: %i", e);
  if(int e=StartProcess(pid[4], CORE_COLLISSION)) 
    ERREXIT2("Could not start process: %i", e);

  if(int e=WaitProcess(pid[0], NULL, CORE_INPUT)) ERREXIT2("Waiting on ping %i@%i: %i\n",   pid[0], 1,  e);
  if(int e=WaitProcess(pid[1], NULL, CORE_PLAYER)) ERREXIT2("Waiting on ping %i@%i: %i\n",  pid[1], 2, e);
  if(int e=WaitProcess(pid[2], NULL, CORE_RENDER)) ERREXIT2("Waiting on ping %i@%i: %i\n",  pid[2], 3, e);
  if(int e=WaitProcess(pid[3], NULL, CORE_ENEMY)) ERREXIT2("Waiting on ping %i@%i: %i\n",   pid[3], 4,  e);
  if(int e=WaitProcess(pid[4], NULL, CORE_COLLISSION)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[4], 5, e);

  printf("Game Over\n");

  return 0;
}
