#include <helix.h>

#include "game_param.h"

#include "core_enemy.h"


void *core_enemy(void *arg)
{
  for (;;)
  {
    usleep(UPDATE_PERIOD);
  }
  return NULL;
}

