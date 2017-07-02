#include <helix.h>

#include "fifo_buffer.h"
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
 * @brief Process to render display
 */
void *core_render(void *arg)
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
