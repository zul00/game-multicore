#include <helix.h>

#include "game_param.h"

#include "core_collission.h"

/**
 * @brief Detect collision
 * @param a Object A to be checked
 * @param b Object B to be checked
 * @return collision status
 */
bool collision_detect(box_area_t a, box_area_t b)
{
  if (a.y + a.h < b.y)
  {
    return 0;
  }
  if (a.y > b.y + b.h)
  {
    return 0;
  }

  if (a.x > b.x + b.w)
  {
    return 0;
  }
  if (a.x + a.w < b.x)
  {
    return 0;
  }
  return 1;
}

void *core_collission(void *arg)
{
  enemy_param_t enemy_param;
  bullet_param_t bullet_param;
  player_param_t player_param;

  bool hit = 0;
  bool f_enemy = 0;
  bool f_bullet = 0;

  /* Initialize */
  // Check FIFO
  rd_enemy->validate("Failed validating");
  rd_player->validate("Failed validating");
  rd_bullet->validate("Failed validating");

  wr_enemy_c->validate("Failed validating");
  wr_player_r->validate("Failed validating");
  wr_bullet_c->validate("Failed validating");

  for (;;)
  {
    if(rd_enemy->count() > 0)
    {
      enemy_param = rd_enemy->front();
      rd_enemy->pop();

      f_enemy = 1;
    }

    if(rd_player->count() > 0)
    {
      player_param = rd_player->front();
      rd_player->pop();

      wr_player_r->push(player_param);
    }

    if(rd_bullet->count() > 0)
    {
      bullet_param = rd_bullet->front();
      rd_bullet->pop();

      f_bullet = 1;
    }

    // Send back
    if (f_enemy && f_bullet)
    {
      hit = collision_detect(enemy_param.box, bullet_param.box);
      if (hit == 1)
      {
        printf("Hit detect = %d\n", hit);
        bullet_param.alive = 0;
        enemy_param.alive = 0;
        hit = 0;
        wr_bullet_c->push(true);
        wr_enemy_c->push(true);
      }
      f_enemy = 0; f_bullet = 0;
    }

    usleep(UPDATE_PERIOD);
  }

  return NULL;
}
