#ifndef __GAME_PARAM_H
#define __GAME_PARAM_H

#include <helix.h>

/* Render config */
#define BOTTOM_POS      DVI_HEIGHT-20

#define PLAYER_HEIGHT   20
#define PLAYER_WIDTH    50

#define BULLET_HEIGHT   10 
#define BULLET_WIDTH    5

typedef struct
{
  coord_t x;
  coord_t y;
  coord_t h;
  coord_t w;
} box_area_t;

typedef struct
{
  box_area_t box;
  int16_t health;
} player_param_t;

typedef struct
{
  box_area_t box;
  bool alive;
} bullet_param_t;

extern CFifo<btn_event_e,CFifo<>::w> *wr_btn;
extern CFifo<btn_event_e,CFifo<>::r> *rd_btn;

extern CFifo<player_param_t,CFifo<>::w> *wr_player;
extern CFifo<player_param_t,CFifo<>::r> *rd_player;

extern CFifo<bullet_param_t,CFifo<>::w> *wr_bullet;
extern CFifo<bullet_param_t,CFifo<>::r> *rd_bullet;

extern CFifo<bullet_param_t,CFifo<>::w> *wr_bullet2;
extern CFifo<bullet_param_t,CFifo<>::r> *rd_bullet2;

#endif
