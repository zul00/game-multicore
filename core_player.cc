#include <helix.h>

#include "fifo_buffer.h"

#include "core_player.h"

/**
 * @brief Player shoot
 */
void player_shoot(bullet_param_t *bullet, player_param_t *player)
{
  // Shoot only if bullet is not alive
  if (bullet->alive == 0) 
  {                                                
    //count number of shots fired
    //score.shots++;

    // Update bullet position w/ player position
    bullet->box.x = player->box.x + (PLAYER_WIDTH / 2);
    bullet->box.y = player->box.y - (bullet->box.h + 10);

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
    bullet->box.y += speed;

    // Bullet out of area BOTTOM
    if (bullet->box.y <= 0) 
    {                                                 
      bullet->alive = 0;
    }

    // Bullet out of area TOP
    if (bullet->box.y + bullet->box.h >= DVI_HEIGHT) 
    {                            
      bullet->alive = 0;
    }
  }

  return 0;
}


/**
 * @brief Process to handle player algorithm
 */
void *core_player(void *arg)
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
  param.box.x = INIT_POS;
  param.box.y = BOTTOM_POS;
  param.box.w = PLAYER_WIDTH;
  param.box.h = PLAYER_HEIGHT;

  // Initialize bullet
  b.box.x = INIT_POS;
  b.box.y = BOTTOM_POS;
  b.box.x = BULLET_WIDTH;
  b.box.y = BULLET_HEIGHT;
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
          if (param.box.x > MIN_POS)
          {
            param.box.x -= INC_POS;
          }
          break;
        case BTN_RIGHT:
          if (param.box.x < MAX_POS)
          {
            param.box.x += INC_POS;
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
