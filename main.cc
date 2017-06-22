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

#define N_CORE 3

/* Render config */
#define BOTTOM_POS      DVI_HEIGHT-20

#define PLAYER_HEIGHT   20
#define PLAYER_WIDTH    50

/* Control Config */
#define INC_POS   20
#define INIT_POS  DVI_WIDTH/2

/* Position Limit */
#define MAX_POS   DVI_WIDTH - PLAYER_WIDTH
#define MIN_POS   0

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

typedef struct
{
  coordinate_t pos;
  coordinate_t size;
} pos_rect_t;

typedef struct
{
  pos_rect_t pos_rect;
  int16_t health;
} player_param_t;

CFifo<btn_event,CFifo<>::w> *wr_btn;
CFifo<btn_event,CFifo<>::r> *rd_btn;


CFifo<player_param_t,CFifo<>::w> *wr_player;
CFifo<player_param_t,CFifo<>::r> *rd_player;

/**
 * @brief Draw player
 */
void draw_player(player_param_t param)
{
  fillrect(
      param.pos_rect.pos.x,                       param.pos_rect.pos.y, 
      param.pos_rect.pos.x+param.pos_rect.size.x, param.pos_rect.pos.y-param.pos_rect.size.y,
      black);
}

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
  player_param_t param;

  printf("Hello Player Alg!!!\n");

  /* Initialize */
  // Check FIFO
  rd_btn->validate();
  wr_player->validate();
  // Initialize player
  param.pos_rect.pos.x = INIT_POS;
  param.pos_rect.pos.y = BOTTOM_POS;
  param.pos_rect.size.x = PLAYER_WIDTH;
  param.pos_rect.size.y = PLAYER_HEIGHT;

  for (;;)
  {
    input = rd_btn->front();
    rd_btn->pop();

    if (input != BTN_NO_EVENT)
    {
      // Player Algorithm
      switch(input)
      {
        case BTN_LEFT:
          if (param.pos_rect.pos.x > MIN_POS)
          {
            param.pos_rect.pos.x -= INC_POS;
          }
          break;
        case BTN_RIGHT:
          if (param.pos_rect.pos.x < MAX_POS)
          {
            param.pos_rect.pos.x += INC_POS;
          }
          break;
        default:
          break;
      }

      param.health  = 100;
      
      printf("button pressed = %s\n ", btn_name[input]);
      wr_player->push((player_param_t) param);
    }
  }

  return NULL;
}

/**
 * @brief Process to render display
 */
void *prc_display(void *arg)
{
  player_param_t input;

  printf("Hello Display!!!\n");

  /* Initialize */
  // Init Render
  render_init(1);

  // Check FIFO
  rd_player->validate();

  // Reset screen
  fillrect(0, 0, DVI_WIDTH, DVI_HEIGHT, orange);
  draw_player(input);
  render_flip_buffer();

  for (;;)
  {
    input = rd_player->front();
    rd_player->pop();

    fillrect(0, 0, DVI_WIDTH, DVI_HEIGHT, orange);
    draw_player(input);
    drawstring(20, 20, "Space Invader", black, -1, -1);   
    render_flip_buffer();
    printf("Render this -> pos = %d, health = %d\n", input.pos_rect.pos.x, input.health);
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
  CFifoPtr<player_param_t> ff_player = CFifo<player_param_t>::Create(2, wr_player, 3, rd_player, 2);
  if(!ff_player.valid()) ERREXIT("Error creating buffer");

  // Create Process
  if(int e=CreateProcess(pid[0], prc_input, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 1))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[1], prc_player_alg, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 2))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[2], prc_display, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 3))
    ERREXIT2("Process creation failed: %i", e);

  // Set Process Flag
  if(int e=SetProcessFlags(pid[0], PROC_FLAG_JOINABLE, 1))
    ERREXIT2("While setting process flags: %i", e);
  if(int e=SetProcessFlags(pid[1], PROC_FLAG_JOINABLE, 2))
    ERREXIT2("While setting process flags: %i", e);
  if(int e=SetProcessFlags(pid[2], PROC_FLAG_JOINABLE, 3))
    ERREXIT2("While setting process flags: %i", e);

  // Start process
  if(int e=StartProcess(pid[0], 1)) 
    ERREXIT2("Could not start process: %i", e);
  if(int e=StartProcess(pid[1], 2)) 
    ERREXIT2("Could not start process: %i", e);
  if(int e=StartProcess(pid[2], 3)) 
    ERREXIT2("Could not start process: %i", e);

  if(int e=WaitProcess(pid[0], NULL, 1)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[0], 1, e);
  if(int e=WaitProcess(pid[1], NULL, 2)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[1], 2, e);
  if(int e=WaitProcess(pid[2], NULL, 3)) ERREXIT2("Waiting on ping %i@%i: %i\n", pid[2], 3, e);

  printf("Game Over\n");

  return 0;
}
