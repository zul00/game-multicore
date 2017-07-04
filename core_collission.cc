#include <helix.h>

#include "game_param.h"

#include "core_collission.h"

void *core_collission(void *arg)
{
//  enemy_param_t enemy_param;
  bullet_param_t bullet_param;
//  player_param_t player_param;
//
//  printf("Hello Player Collission!!!\n");
//
//  /* Initialize */
//  // Check FIFO
//  rd_enemy->validate();
//  rd_player->validate();
  rd_bullet->validate("Failed validating");
//
////  wr_enemy_r->validate();
////  wr_player_r->validate();
////  wr_bullet_r->validate();
//
  for (;;)
  {
//    if(rd_enemy->count() > 0)
//    {
//      enemy_param = rd_enemy->front();
//      rd_enemy->pop();
//
//      printf("count enemy = %d\n", rd_enemy->count());
//
//      //wr_enemy_r->push(enemy_param);
//    }
//
//    if(rd_player->count() > 0)
//    {
//      player_param = rd_player->front();
//      rd_player->pop();
//
//      printf("count player = %d\n", rd_player->count());
//
////      wr_player_r->push(player_param);
//    }
//
    if(rd_bullet->count() > 0)
    {
      bullet_param = rd_bullet->front();
      rd_bullet->pop();

      wr_bullet_r->push(bullet_param);
    }

    usleep(UPDATE_PERIOD);
  }

  return NULL;
}
