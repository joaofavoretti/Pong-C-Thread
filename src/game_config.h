#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#define ERROR -1
#define TICK_DELAY 80   /* Number of milliseconds do tick ball and oponent. */
#define KEYBOARD_ENTER '\n'
#define QUIT_GAME 'q'
#define RESTART_GAME 'r'

#include <ncurses.h>

extern struct timeval start, now; /* {tv_sec = seconds, tv_usec = nanoseconds} */

int init_ncurses_config(WINDOW *root, WINDOW *stdscr);
int display_start_menu();
int update_time(struct timeval *time);
unsigned int time_difference(struct timeval *start, struct timeval *now);
int show_ingame_menu();
int show_ingame_scores();
int show_ingame_line();

#endif
