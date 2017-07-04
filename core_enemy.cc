#include <helix.h>

#include "game_param.h"

#include "core_enemy.h"


void *core_enemy(void *arg)
{
  enemy_param_t enemy_param;
  bool enemy_just_died = 0;

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
    wr_enemy->push(enemy_param);

    if(rd_enemy_c->count() > 0)
    {
      enemy_param = rd_enemy_c->front();
      if (enemy_param.alive == 0)
      {
        enemy_just_died = 1;
      }
      rd_enemy_c->pop();
    }

    if(enemy_param.alive == true || enemy_just_died == 1)
    {
      wr_enemy_r->push(enemy_param);
      enemy_just_died = 0;
    }

    usleep(UPDATE_PERIOD);
  }
  return NULL;
}
