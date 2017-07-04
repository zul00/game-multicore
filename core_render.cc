#include <helix.h>

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
        red);
  }
}

/**
 * @brief Draw player
 */
void draw_player(player_param_t param)
{
  fillrect(
      param.box.x,              param.box.y, 
      param.box.x+param.box.w,  param.box.y-param.box.h,
      black);
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
        green);
  }
}

/**
 * @brief Process to render display
 */
void *core_render(void *arg)
{
  player_param_t player_param;
  bullet_param_t pbullet_param;
  enemy_param_t enemy_param;

//  printf("Hello Display!!!\n");

  /* Initialize */
  // Init Render
  render_init(1);

  // Check FIFO
  rd_player->validate();
  rd_bullet->validate();
  rd_enemy->validate();

  // Reset screen
  fillrect(0, 0, DVI_WIDTH, DVI_HEIGHT, orange);
  draw_player(player_param);
  render_flip_buffer();

  for (;;)
  {
    /* Check FIFO */
    if (rd_player->count())
    {
      player_param  = rd_player->front();
      rd_player->pop();
    }

    if (rd_bullet->count())
    {
      pbullet_param = rd_bullet->front();
      rd_bullet->pop();
    }
    //printf("count bullet = %d\n", rd_bullet->count());
    if (rd_enemy->count())
    {
      enemy_param = rd_enemy->front();
      rd_enemy->pop();
    }

    /* Draw Stuffs */
    // Background
    fillrect(0, 0, DVI_WIDTH, DVI_HEIGHT, orange);
    // Player
    draw_player(player_param);
    // Enemy
    draw_enemy(enemy_param);
    // Bullet
    draw_bullets(pbullet_param);
    // Stats
    drawstring(20, 20, "Space Invader", black, -1, -1);   

    render_flip_buffer();

    usleep(RENDER_PERIOD);
  }

  return NULL;
}
