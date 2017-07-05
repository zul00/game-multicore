#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <helix.h>

#include "game_param.h"

#include "core_enemy.h"

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
  enemy_param_t enemy_param;
  bullet_param_t bullet_param;
  bool bullet_alive_prev = 0;
  bool enemy_alive_prev = 0;
  time_t t;

//  printf("Hello Enemy Alg!!!\n");

  /* Initialize */
  // Check FIFO
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
  enemy_param.alive = 1;

  // Initialize random
  srand((unsigned) time(&t));

  for (;;)
  {
    if(rd_enemy_c->count() > 0)
    {
      enemy_param.alive = !rd_enemy_c->front();
      rd_enemy_c->pop();
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
    enemy_shoot(&bullet_param, &enemy_param);
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
