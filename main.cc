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

#define N_CORE 4

/* Render config */
#define BOTTOM_POS      DVI_HEIGHT-20

#define PLAYER_HEIGHT   20
#define PLAYER_WIDTH    50

#define BULLET_HEIGHT   10 
#define BULLET_WIDTH    5

/* Control Config */
#define INC_POS   20
#define INIT_POS  DVI_WIDTH/2

#define P_BULLETS 1

/* Position Limit */
#define MAX_POS   DVI_WIDTH - PLAYER_WIDTH
#define MIN_POS   0

/* Timing configureation */
#define BTN_SCAN_PERIOD   50000   // 50 ms



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

typedef struct
{
  pos_rect_t pos_rect;
  bool alive;
} bullet_param_t;


/* Declare FIFO buffer */
CFifo<btn_event_e,CFifo<>::w> *wr_btn;
CFifo<btn_event_e,CFifo<>::r> *rd_btn;

CFifo<player_param_t,CFifo<>::w> *wr_player;
CFifo<player_param_t,CFifo<>::r> *rd_player;

CFifo<bullet_param_t,CFifo<>::w> *wr_bullet;
CFifo<bullet_param_t,CFifo<>::r> *rd_bullet;

CFifo<bullet_param_t,CFifo<>::w> *wr_bullet2;
CFifo<bullet_param_t,CFifo<>::r> *rd_bullet2;

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
 * @brief Player shoot
 */
void player_shoot(bullet_param_t *bullet, player_param_t *player)
{
  if (bullet->alive == 0) 
  {                                                
    //count number of shots fired                                             
    //score.shots++;                                                            

    bullet->pos_rect.pos.x = player->pos_rect.pos.x + (PLAYER_WIDTH / 2);                    
    //-5 at the end so the bullets ends closer to the top of the screen due to 30px speed
    bullet->pos_rect.pos.y = player->pos_rect.pos.y - (bullet->pos_rect.size.y + 10);       
    bullet->alive = 1;                                                     
  }                                                                           
}

/**
 * @brief Move bullet
 */
int move_bullets(bullet_param_t *bullet, int speed) 
{                     
  if (bullet->alive == 1) 
  {                                                      
    bullet->pos_rect.pos.y += speed;

    // Bullet out of area BOTTOM
    if (bullet->pos_rect.pos.y <= 0) 
    {                                                 
      bullet->alive = 0;
    }                                                                         

    // Bullet out of area TOP
    if (bullet->pos_rect.pos.y + bullet->pos_rect.size.y >= DVI_HEIGHT) 
    {                            
      bullet->alive = 0;
    }                                                                         
  }                                                                             

  return 0;                                                                     
}

/**
 * @brief Draw bullets
 */
void draw_bullets(bullet_param_t bullet) 
{                               
  if (bullet.alive == 1) 
  {                                                      
    fillrect(
        bullet.pos_rect.pos.x,                       bullet.pos_rect.pos.y, 
        bullet.pos_rect.pos.x+bullet.pos_rect.size.x, bullet.pos_rect.pos.y-bullet.pos_rect.size.y,
        red);
  }                                                                           
}


/**
 * @brief Process to handle player algorithm
 */
void *prc_player_alg(void *arg)
{
  uint16_t input = 0;
  player_param_t param;
  bullet_param_t b;

  printf("Hello Player Alg!!!\n");

  /* Initialize */
  // Check FIFO
  rd_btn->validate();
  wr_player->validate();
  wr_bullet->validate();

  // Initialize player
  param.pos_rect.pos.x = INIT_POS;
  param.pos_rect.pos.y = BOTTOM_POS;
  param.pos_rect.size.x = PLAYER_WIDTH;
  param.pos_rect.size.y = PLAYER_HEIGHT;

  // Initialize bullet
  b.pos_rect.pos.x = INIT_POS;
  b.pos_rect.pos.y = BOTTOM_POS;
  b.pos_rect.size.x = BULLET_WIDTH;
  b.pos_rect.size.y = BULLET_HEIGHT;
  b.alive = 0;

  for (;;)
  {
    // Read button FIFO if there's data
    if(rd_btn->count() > 0)
    {
      input = rd_btn->front();
      rd_btn->pop();
    }
    else
    {
      input = BTN_NO_EVENT;
    }

    // Handle button FIFO
    if (input != BTN_NO_EVENT)
    {
      // Player Algorithm
      // Update position
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
        case BTN_ENTER:
          player_shoot(&b, &param);
          
          break;
        default:
          break;
      }
      param.health  = 100;

      printf("button pressed = %s\n ", btn_string[input]);
      wr_player->push((player_param_t) param);
    }

    // Update other things
    move_bullets(&b, -15);
    wr_bullet->push((bullet_param_t) b);
    usleep(50000);
  }

  return NULL;
}

/**
 * @brief Process to render display
 */
void *prc_update_pos(void *arg)
{
  //bullet_param_t b_pos;

  printf("Hello Update Pos!!!\n");

  /* Initialize */

//  rd_bullet->validate();
//  wr_bullet2->validate();

  for (;;)
  {
//    if(rd_bullet->count() > 0)
//    {
//      b_pos = rd_bullet->front();
//      rd_bullet->pop();
//    }
//
//    printf("bullet_y0 = %d\n", b_pos.pos_rect.pos.y);
//    move_bullets(&b_pos, -5);
//    printf("bullet_y1 = %d\n", b_pos.pos_rect.pos.y);
//
//    wr_bullet2->push((bullet_param_t) b_pos);
//
    usleep(33000);
  }

  return NULL;
}

/**
 * @brief Process to render display
 */
void *prc_display(void *arg)
{
  player_param_t input;
  bullet_param_t b_draw;

  printf("Hello Display!!!\n");

  /* Initialize */
  // Init Render
  render_init(1);

  // Check FIFO
  rd_player->validate();
  rd_bullet->validate();

  // Reset screen
  fillrect(0, 0, DVI_WIDTH, DVI_HEIGHT, orange);
  draw_player(input);
  render_flip_buffer();

  for (;;)
  {
    if (rd_player->count())
    {
      input  = rd_player->front();
      rd_player->pop();
    }

    if (rd_bullet->count())
    {
      b_draw = rd_bullet->front();
      rd_bullet->pop();
    }
    printf("count bullet = %d\n", rd_bullet->count());

    fillrect(0, 0, DVI_WIDTH, DVI_HEIGHT, orange);
    draw_player(input);
    draw_bullets(b_draw);
    drawstring(20, 20, "Space Invader", black, -1, -1);   
    render_flip_buffer();
    usleep(3300);
  }

  return NULL;
}

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
//  CFifoPtr<bullet_param_t> ff_bullet2 = CFifo<bullet_param_t>::Create(4, wr_bullet2, 3, rd_bullet2, 2);
//  if(!ff_bullet2.valid()) ERREXIT("Error creating buffer");

  // Create Process
  if(int e=CreateProcess(pid[0], core_input, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 1))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[1], prc_player_alg, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 2))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[2], prc_display, NULL, PROC_DEFAULT_TIMESLICE,
        PROC_DEFAULT_STACK, 3))
    ERREXIT2("Process creation failed: %i", e);
  if(int e=CreateProcess(pid[3], prc_update_pos, NULL, PROC_DEFAULT_TIMESLICE,
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
