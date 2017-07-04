#include <helix.h>

#include "game_param.h"

#include "core_player.h"

/**
 * @brief Player shoot
 */
void player_shoot(bullet_param_t *bullet, player_param_t *player)
{
  // Shoot only if there is still bullet that is NOT alive 
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
  player_param_t player_param;
  bullet_param_t bullet_param;

//  printf("Hello Player Alg!!!\n");

  /* Initialize */
  // Check FIFO
  rd_btn->validate();
  wr_player->validate();
  wr_bullet->validate();

  // Initialize player
  player_param.box.x = INIT_POS;
  player_param.box.y = BOTTOM_POS;
  player_param.box.w = PLAYER_WIDTH;
  player_param.box.h = PLAYER_HEIGHT;

  // Initialize bullet
  bullet_param.box.x = INIT_POS;
  bullet_param.box.y = BOTTOM_POS;
  bullet_param.box.w = BULLET_WIDTH;
  bullet_param.box.h = BULLET_HEIGHT;
  bullet_param.alive = 0;

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
          if (player_param.box.x > MIN_POS)
          {
            player_param.box.x -= INC_POS;
          }
          break;

        case BTN_RIGHT:
          if (player_param.box.x < MAX_POS)
          {
            player_param.box.x += INC_POS;
          }
          break;

        case BTN_ENTER:
          player_shoot(&bullet_param, &player_param);
          break;

        default:
          break;
      }
      player_param.health  = 100;

      printf("button pressed = %s\n ", btn_string[input]);
      wr_player->push(player_param);
    }

    // Update other things
    move_bullets(&bullet_param, -15);
    wr_bullet->push(bullet_param);

    usleep(UPDATE_PERIOD);
  }

  return NULL;
}
