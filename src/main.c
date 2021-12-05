#include <ncurses.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <window.h>
#include <game_config.h>
#include <player.h>
#include <ball.h>

/* Global variables */
pong_ball ball;
pong_player player, opponent;
struct timeval start, now;
WINDOW *field;
int player_default_height,
    player_default_width,
    player_left_start_y,
    player_right_start_y,
    player_left_start_x,
    player_right_start_x;

int main()
{
  int ch = 0;
  bool done = false;

  /* Inicia seed */
  srand(time(NULL));

  /* INIT NCURSES */
  /* Inicia os valores de COLS e LINES com o tamanho do terminal */
  WINDOW *root = initscr();

  /* Inicia os parametros do ncurses */
  init_ncurses_config(root, stdscr);

  /* INIT PLAYERS CONFIG */
  init_players_config();

  /* Atualiza a tela com os parametros setados */
  refresh();

  /* Mostra menu inicial e espera ação */
  display_start_menu();
  wclear(stdscr);

  refresh();

  /* Iniciar oponente - Deve ser feito depois de init_players_config */
  init_pong_opponent();

  /* Iniciar player principal - Deve ser feito depois de init_players_config */
  init_pong_player();

  /* Iniciar bola */
  init_pong_ball();

  refresh();

  /* Inicia a struct timeval start para medir o tamanho do TICK */
  update_time(&start);

  /* Loop principal */
  while (!done)
  {
    /* Usa a entrada do usuario se tiver */
    ch = getch();

    switch (ch)
    {
    case PLAYER_UP: // Move player up
      if (player.window.posY > 0) {
        player.window.posY--;
        update_pong_player();
      }
      break;
    case PLAYER_DOWN: // Move player down
      if (player.window.posY + player.window.height < LINES) {
        player.window.posY++;
        update_pong_player();
      }
      break;
    case RESTART_GAME: // Reset the game
      destroy_pong_player();
      init_pong_player();

      destroy_pong_opponent();
      init_pong_opponent();

      destroy_pong_ball();
      init_pong_ball();
      break;
    case QUIT_GAME: // Quit Game
      done = true;
      break;
    }

    /* Exibir os scores */
    show_ingame_scores();

    /* Desenhar a linha central */
    show_ingame_line();

    update_time(&now);
    unsigned int diff = time_difference(&start, &now);

    /* Atualiza ball e opponent caso necessário */
    if (diff >= TICK_DELAY)
    {
      auto_move_ball();
      auto_move_opponent();
      start = now;
    }
  }

  /* CLOSE NCURSES */
  if (endwin() == ERR)
  {
    fprintf(stderr, "ERROR: endwin()");
    exit(ERROR);
  }

  return 0;
}
