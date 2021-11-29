#ifndef PLAYER_H
#define PLAYER_H

#include <window.h>

#define PLAYER_UP 'w'
#define PLAYER_DOWN 's'

typedef struct {
    pong_win window;
    int score;
} pong_player;

extern pong_player player, opponent;

extern int 
    player_default_height,
    player_default_width,
    player_left_start_y,
    player_right_start_y,
    player_left_start_x,
    player_right_start_x;

int init_players_config();
int auto_move_opponent();
int init_pong_opponent();
int init_pong_player();
int destroy_pong_opponent();
int destroy_pong_player();
int update_pong_opponent();
int update_pong_player();

#endif
