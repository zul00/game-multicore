#include <helix.h>

#include "game_param.h"

#include "core_enemy.h"


void *core_enemy(void *arg)
{
  enemy_param_t enemy_param;

  printf("Hello Player Alg!!!\n");

  /* Initialize */
  // Check FIFO
  wr_enemy->validate();

  // Initialize enemy
  enemy_param.box.x = INIT_POS;
  enemy_param.box.y = TOP_POS;
  enemy_param.box.w = ENEMY_WIDTH;
  enemy_param.box.h = ENEMY_HEIGHT;
  enemy_param.alive = 1;

  for (;;)
  {
    wr_enemy->push(enemy_param);

    usleep(UPDATE_PERIOD);
  }
  return NULL;
}
