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
  bool bullet_alive_prev = 0;
  bool player_alive_prev = 0;
  player_param_t player_param;
  bullet_param_t bullet_param;

//  printf("Hello Player Alg!!!\n");

  /* Initialize */
  // Check FIFO
  rd_btn->validate("Failed validating");
  wr_player->validate("Failed validating");
  rd_player_c->validate("Failed validating");
  wr_player_r->validate("Failed validating");
  wr_bullet->validate("Failed validating");
  wr_bullet_r->validate("Failed validating");

  // Initialize player
  player_param.box.x = INIT_POS;
  player_param.box.y = BOTTOM_POS;
  player_param.box.w = PLAYER_WIDTH;
  player_param.box.h = PLAYER_HEIGHT;
  player_param.alive = true;

  // Initialize bullet
  bullet_param.box.x = INIT_POS;
  bullet_param.box.y = BOTTOM_POS;
  bullet_param.box.w = BULLET_WIDTH;
  bullet_param.box.h = BULLET_HEIGHT;
  bullet_param.alive = false;

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

    // Check for collision
    if(rd_player_c->count() > 0)
    {
      player_param.alive = !rd_player_c->front();
      rd_player_c->pop();

      printf("Collision event Detected\n");
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

      printf("button pressed = %s\n ", btn_string[input]);
    }

    // Update other things
    move_bullets(&bullet_param, -15);
    if (rd_bullet_c->count() > 0)
    {
      bullet_param.alive = !rd_bullet_c->front();
      rd_bullet_c->pop();
    }

    // Push player
    if (player_param.alive == true)
    {
      wr_player->push(player_param);
      wr_player_r->push(player_param);
    }
    else
    {
      if (player_alive_prev)
      {
        wr_player_r->push(player_param);
        printf("Player last update\n");
      }
    }

    // Push bullet
    if (bullet_param.alive == true)
    {
        wr_bullet->push(bullet_param);
        wr_bullet_r->push(bullet_param);
    }
    else
    {
      if (bullet_alive_prev)
      {
        wr_bullet_r->push(bullet_param);
        printf("Bullet last update\n");
      }
    }

    // Update previous value
    bullet_alive_prev = bullet_param.alive;

    usleep(UPDATE_PERIOD);
  }

  return NULL;
}
