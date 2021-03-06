#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <helix.h>

#include "game_param.h"

#include "core_enemy.h"

#define INC_ENEMY 5

/**
 * @brief Enemy shoot
 */
void enemy_shoot(bullet_param_t *bullet, enemy_param_t *enemy)
{
  // Shoot only if there is still bullet that is NOT alive 
  if (bullet->alive == 0) 
  {                                                
    //count number of shots fired
    //score.shots++;

    // Update bullet position w/ enemy position
    bullet->box.x = enemy->box.x + (ENEMY_WIDTH / 2);
    bullet->box.y = enemy->box.y - (bullet->box.h + 10);

    bullet->alive = 1; 
  }
}

/**
 * @brief Move bullet
 */
int move_ebullets(bullet_param_t *bullet, int speed) 
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

void *core_enemy(void *arg)
{
  uint16_t input = 0;
  enemy_param_t enemy_param;
  bullet_param_t bullet_param;
  bool bullet_alive_prev = 0;
  bool enemy_alive_prev = 0;
  time_t t;
  uint16_t enemy_dir = 0;

//  printf("Hello Enemy Alg!!!\n");

  /* Initialize */
  // Check FIFO
  rd_btne->validate("Failed validating");
  wr_enemy->validate("Failed validating");
  wr_enemy_r->validate("Failed validating");
  rd_enemy_c->validate("Failed validating");
  wr_ebullet->validate("Failed validating");
  wr_ebullet_r->validate("Failed validating");

  // Initialize enemy
  enemy_param.box.x = INIT_POS;
  enemy_param.box.y = TOP_POS;
  enemy_param.box.w = ENEMY_WIDTH;
  enemy_param.box.h = ENEMY_HEIGHT;
  enemy_param.alive = true;

  // Initialize bullet
  bullet_param.box.x = INIT_POS;
  bullet_param.box.y = TOP_POS;
  bullet_param.box.w = BULLET_WIDTH;
  bullet_param.box.h = BULLET_HEIGHT;
  bullet_param.alive = false;

  // Initialize random
  srand((unsigned) time(&t));

  for (;;)
  {
    // Check for collision
    if(rd_enemy_c->count() > 0)
    {
      enemy_param.alive = !rd_enemy_c->front();
      rd_enemy_c->pop();
    }

    // Read button FIFO if there's data
    if(rd_btne->count() > 0)
    {
      printf("Event detected\n");
      input = rd_btne->front();
      rd_btne->pop();
    }
    else
    {
      input = BTN_NO_EVENT;
    }

    if (input == BTN_DOWN)
    {
      printf("Here!!!\n");
      // Initialize enemy
      enemy_param.box.x = INIT_POS;
      enemy_param.box.y = TOP_POS;
      enemy_param.box.w = ENEMY_WIDTH;
      enemy_param.box.h = ENEMY_HEIGHT;
      enemy_param.alive = true;

      // Initialize bullet
      bullet_param.box.x = INIT_POS;
      bullet_param.box.y = TOP_POS;
      bullet_param.box.w = BULLET_WIDTH;
      bullet_param.box.h = BULLET_HEIGHT;
      bullet_param.alive = false;

    }

    if (enemy_dir == 0)
    {
      if (enemy_param.box.x+ENEMY_WIDTH < DVI_WIDTH)
      {
        enemy_param.box.x += INC_ENEMY;
      }
      else
      {
        enemy_dir = 1;
      }
    }
    else
    {
      if (enemy_param.box.x > 0)
      {
        enemy_param.box.x -= INC_ENEMY;
      }
      else
      {
        enemy_dir = 0;
      }
    }

    // Push enemy
    if (enemy_param.alive == true)
    {
      wr_enemy->push(enemy_param);
      wr_enemy_r->push(enemy_param);
    }
    else
    {
      if (enemy_alive_prev)
      {
        wr_enemy_r->push(enemy_param);
        printf("Enemy last update\n");
      }
    }

    // Enemy shoot
    if (enemy_param.alive)
    {
      enemy_shoot(&bullet_param, &enemy_param);
    }
    move_ebullets(&bullet_param, 15);
    if (rd_ebullet_c->count() > 0)
    {
      bullet_param.alive = !rd_ebullet_c->front();
      rd_ebullet_c->pop();
    }
    // Push bullet
    if (bullet_param.alive == true)
    {
        wr_ebullet->push(bullet_param);
        wr_ebullet_r->push(bullet_param);
    }
    else
    {
      if (bullet_alive_prev)
      {
        wr_ebullet_r->push(bullet_param);
        printf("Bullet last update\n");
      }
    }

    // Update previous value
    bullet_alive_prev = bullet_param.alive;
    enemy_alive_prev = enemy_param.alive;

    usleep(UPDATE_PERIOD);
  }
  return NULL;
}
