#include <helix.h>

#include "game_param.h"

#include "core_enemy.h"


void *core_enemy(void *arg)
{
  enemy_param_t enemy_param;
  bool enemy_alive_prev = 0;

//  printf("Hello Enemy Alg!!!\n");

  /* Initialize */
  // Check FIFO
  wr_enemy->validate("Failed validating");
  wr_enemy_r->validate("Failed validating");
  rd_enemy_c->validate("Failed validating");

  // Initialize enemy
  enemy_param.box.x = INIT_POS;
  enemy_param.box.y = TOP_POS;
  enemy_param.box.w = ENEMY_WIDTH;
  enemy_param.box.h = ENEMY_HEIGHT;
  enemy_param.alive = 1;

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

    // Update previous value
    enemy_alive_prev = enemy_param.alive;

    usleep(UPDATE_PERIOD);
  }
  return NULL;
}
