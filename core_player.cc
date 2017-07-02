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
    bullet->pos_rect.pos.x = player->pos_rect.pos.x + (PLAYER_WIDTH / 2);
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
