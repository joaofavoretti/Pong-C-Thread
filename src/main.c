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
#include <semaphore.h>
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
sem_t semaphore;
unsigned int diff;

void *ball_thread_routine(void* args) {
  sem_t *semaphore = (sem_t *) args;
  sem_wait(semaphore);
  auto_move_ball();
  sem_post(semaphore);
  return NULL;
}

void *oponent_thread_routine(void* args) {
  sem_t *semaphore = (sem_t *) args;
  sem_wait(semaphore);
  auto_move_opponent();
  sem_post(semaphore);
  return NULL;
}

pthread_t create_ball_thread(sem_t *semaphore) {
  pthread_t thread_id;
  if (pthread_create(&thread_id, NULL, &ball_thread_routine, semaphore))
      fprintf(stderr, "Error to create ball thread!\n");
  return thread_id;
}

pthread_t create_oponent_thread(sem_t *semaphore) {
  pthread_t thread_id;
  if (pthread_create(&thread_id, NULL, &oponent_thread_routine, semaphore))
      fprintf(stderr, "Error to create oponent thread!\n");
  return thread_id;
}

void join_ball_thread(pthread_t thread_id) {
  if (pthread_join(thread_id, NULL))
      fprintf(stderr, "Error to join ball thread!\n");
}

void join_oponent_thread(pthread_t thread_id) {
  if (pthread_join(thread_id, NULL))
      fprintf(stderr, "Error to join oponent thread!\n");
}

int main()
{
  int ch = 0;
  bool done = false;
  diff = 0;

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
    diff = time_difference(&start, &now);

    /* Atualiza ball e opponent caso necessário */
    if (diff >= TICK_DELAY)
    {
      sem_init(&semaphore, 0, 1);
      pthread_t ball_thread_id = create_ball_thread(&semaphore);
      pthread_t oponent_thread_id = create_oponent_thread(&semaphore);
      join_ball_thread(ball_thread_id);
      join_oponent_thread(oponent_thread_id);
      sem_destroy(&semaphore);
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
