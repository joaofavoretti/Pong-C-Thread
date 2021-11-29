#ifndef WINDOW_H
#define WINDOW_H

typedef struct
{
  int posX, posY;
  int height, width;
  WINDOW *win;
} pong_win;

extern WINDOW *field;     /* Campo do jogo "pong" */

WINDOW *create_newwin(int hepight, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

#endif