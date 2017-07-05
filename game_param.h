#ifndef __GAME_PARAM_H
#define __GAME_PARAM_H

#include <helix.h>

#include "core_input.h"

/* Config Control */
#define INC_POS   20

/* Config Parameter */
// Player
#define PLAYER_HEIGHT   10
#define PLAYER_WIDTH    25

#define INIT_POS        DVI_WIDTH/2
#define P_BULLETS 1
#define E_BULLETS 1

// Enemy
#define ENEMY_HEIGHT  20
#define ENEMY_WIDTH   40

#define TOP_POS       20+ENEMY_HEIGHT

/* Position Limit */
#define MAX_POS   DVI_WIDTH - PLAYER_WIDTH
#define MIN_POS   0

/* Render config */
#define BOTTOM_POS      DVI_HEIGHT-20


#define BULLET_HEIGHT   10 
#define BULLET_WIDTH    5

/* Timing configureation */
#define BTN_SCAN_PERIOD   50000   // 50 ms
#define UPDATE_PERIOD     33000   // 50 ms
#define RENDER_PERIOD     3300    // 3.3 ms

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
  bool alive;
} player_param_t;

typedef struct
{
  box_area_t box;
  bool alive;
} enemy_param_t;

typedef struct
{
  box_area_t box;
  bool alive;
} bullet_param_t;

extern CFifo<btn_event_e,CFifo<>::w> *wr_btn;
extern CFifo<btn_event_e,CFifo<>::r> *rd_btn;

extern CFifo<btn_event_e,CFifo<>::w> *wr_btne;
extern CFifo<btn_event_e,CFifo<>::r> *rd_btne;

extern CFifo<player_param_t,CFifo<>::w> *wr_player;
extern CFifo<player_param_t,CFifo<>::r> *rd_player;

extern CFifo<bool,CFifo<>::w> *wr_player_c;
extern CFifo<bool,CFifo<>::r> *rd_player_c;

extern CFifo<player_param_t,CFifo<>::w> *wr_player_r;
extern CFifo<player_param_t,CFifo<>::r> *rd_player_r;

extern CFifo<enemy_param_t,CFifo<>::w> *wr_enemy;
extern CFifo<enemy_param_t,CFifo<>::r> *rd_enemy;

extern CFifo<bool,CFifo<>::w> *wr_enemy_c;
extern CFifo<bool,CFifo<>::r> *rd_enemy_c;

extern CFifo<enemy_param_t,CFifo<>::w> *wr_enemy_r;
extern CFifo<enemy_param_t,CFifo<>::r> *rd_enemy_r;

extern CFifo<bullet_param_t,CFifo<>::w> *wr_bullet;
extern CFifo<bullet_param_t,CFifo<>::r> *rd_bullet;

extern CFifo<bool,CFifo<>::w> *wr_bullet_c;
extern CFifo<bool,CFifo<>::r> *rd_bullet_c;

extern CFifo<bullet_param_t,CFifo<>::w> *wr_bullet_r;
extern CFifo<bullet_param_t,CFifo<>::r> *rd_bullet_r;

extern CFifo<bullet_param_t,CFifo<>::w> *wr_ebullet;
extern CFifo<bullet_param_t,CFifo<>::r> *rd_ebullet;

extern CFifo<bool,CFifo<>::w> *wr_ebullet_c;
extern CFifo<bool,CFifo<>::r> *rd_ebullet_c;

extern CFifo<bullet_param_t,CFifo<>::w> *wr_ebullet_r;
extern CFifo<bullet_param_t,CFifo<>::r> *rd_ebullet_r;

#endif
