#include <helix.h>

#include <string.h>
#include <time.h>

#include "game_param.h"

#include "core_input.h"

#include "core_render.h"

/**
 * @brief Draw bullets
 */
void draw_bullets(bullet_param_t bullet) 
{                               
  if (bullet.alive == 1) 
  {                                                      
    fillrect(
        bullet.box.x,               bullet.box.y, 
        bullet.box.x+bullet.box.w,  bullet.box.y-bullet.box.h,
        white);
  }
}

/**
 * @brief Draw player
 */
void draw_player(player_param_t param)
{
  if (param.alive != 0)
  {
    fillrect(
        param.box.x,              param.box.y, 
        param.box.x+param.box.w,  param.box.y-param.box.h,
        yellow);
  }
}

/**
 * @brief Draw enemy
 */
void draw_enemy(enemy_param_t param)
{
  if (param.alive != 0)
  {
    fillrect(
        param.box.x,              param.box.y, 
        param.box.x+param.box.w,  param.box.y-param.box.h,
        red);
  }
}

/**
 * @brief Process to render display
 */
void *core_render(void *arg)
{
  player_param_t player_param;
  bullet_param_t pbullet_param;
  bullet_param_t ebullet_param;
  enemy_param_t enemy_param;
  struct timespec start = {0,0}, finish={0,0}; 
  double elapsed; 
  char string[20];

  bool f_enemy = false;
  bool f_player = false;
  bool f_bullet = false;
  bool f_ebullet = false;

//  printf("Hello Display!!!\n");

  /* Initialize */
  // Init Render
  render_init(1);

  // Check FIFO
  rd_player_r->validate("Failed validating");
  rd_bullet_r->validate("Failed validating");
  rd_ebullet_r->validate("Failed validating");
  rd_enemy_r->validate("Failed validating");

  // Reset screen
  fillrect(0, 0, DVI_WIDTH, DVI_HEIGHT, black);
  render_flip_buffer();

  for (;;)
  {
    clock_gettime(CLOCK_MONOTONIC, &start);
    elapsed = (start.tv_sec - finish.tv_sec);
    elapsed += (start.tv_nsec - finish.tv_nsec) / 1000000000.0;
    sprintf(string, "FPS = %.0f", 1/elapsed);
    finish = start;
    
    /* Check FIFO */
    if (rd_player_r->count())
    {
      player_param  = rd_player_r->front();
      rd_player_r->pop();

      f_player = true;
    }

    if (rd_bullet_r->count())
    {
      pbullet_param = rd_bullet_r->front();
      rd_bullet_r->pop();

      f_bullet = true;
    }

    if (rd_ebullet_r->count())
    {
      ebullet_param = rd_ebullet_r->front();
      rd_ebullet_r->pop();

      f_ebullet = true;
    }

    if (rd_enemy_r->count())
    {
      enemy_param = rd_enemy_r->front();
      rd_enemy_r->pop();

      f_enemy = true;
    }

    /* Draw Stuffs */
    // Background
    fillrect(0, 0, DVI_WIDTH, DVI_HEIGHT, black);
    drawstring(500, 20, string, green, -1, -1);   
    // Bullet
    if (f_bullet)
    {draw_bullets(pbullet_param);}
    if (f_ebullet)
    {draw_bullets(ebullet_param);}
    // Player
    if (f_player)
    {draw_player(player_param);}
    // Enemy
    if (f_enemy)
    {draw_enemy(enemy_param);}
    // Stats
    drawstring(20, 20, "Space Invader", pink, -1, -1);   
    if (!enemy_param.alive)
    {
      drawstring(DVI_WIDTH/2, (DVI_HEIGHT/2)+20, "WIN!!!", pink, -1, -1);   
    }
    if (!player_param.alive)
    {
      drawstring(DVI_WIDTH/2, DVI_HEIGHT/2, "GAME OVER!!!", red, -1, -1);   
    }
    render_flip_buffer();

    usleep(RENDER_PERIOD);
  }

  return NULL;
}
