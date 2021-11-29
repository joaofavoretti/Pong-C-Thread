#ifndef BALL_H
#define BALL_H

typedef struct {
  pong_win window;
  int vx;
  int vy;
  int start_dyx;
  int start_dyy;
  bool collided;
} pong_ball;

extern pong_ball ball;

int auto_move_ball();
int reset_ball();
int init_pong_ball();
int destroy_pong_ball();
int update_pong_ball();

#endif
