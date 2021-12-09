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
WINDOW *field;
int player_default_height,
    player_default_width,
    player_left_start_y,
    player_right_start_y,
    player_left_start_x,
    player_right_start_x;
sem_t semaphore;

void *ball_thread_routine(void *args)
{
  sem_t *semaphore = (sem_t *) args;
  struct timeval start, now;
  update_time(&start);
  while (true)
  {
    sem_wait(semaphore);
    update_time(&now);
    if (time_difference(&start, &now) >= TICK_DELAY)
    {
      auto_move_ball();
      start = now;
    }
    sem_post(semaphore);
  }
  return NULL;
}

void *opponent_thread_routine(void *args)
{
  sem_t *semaphore = (sem_t *) args;
  struct timeval start, now;
  update_time(&start);
  while (true)
  {
    sem_wait(semaphore);
    update_time(&now);
    if (time_difference(&start, &now) >= TICK_DELAY)
    {
      auto_move_opponent();
      start = now;
    }
    sem_post(semaphore);
  }
  return NULL;
}

pthread_t create_ball_thread(sem_t *semaphore)
{
  pthread_t thread_id;
  if (pthread_create(&thread_id, NULL, &ball_thread_routine, semaphore))
  {
    fprintf(stderr, "Error to create ball thread!\n");
  }
  return thread_id;
}

pthread_t create_opponent_thread(sem_t *semaphore)
{
  pthread_t thread_id;
  if (pthread_create(&thread_id, NULL, &opponent_thread_routine, semaphore))
  {
    fprintf(stderr, "Error to create opponent thread!\n");
  }
  return thread_id;
}

void join_ball_thread(pthread_t thread_id)
{
  if (pthread_join(thread_id, NULL))
  {
    fprintf(stderr, "Error to join ball thread!\n");
  }
}

void join_opponent_thread(pthread_t thread_id)
{
  if (pthread_join(thread_id, NULL))
  {
    fprintf(stderr, "Error to join opponent thread!\n");
  }
}

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

  /* Inicia as threads para movimentação da bola e do oponente */
  sem_init(&semaphore, 0, 1);
  pthread_t ball_thread_id = create_ball_thread(&semaphore);
  pthread_t opponent_thread_id = create_opponent_thread(&semaphore);

  /* Loop principal */
  while (!done)
  {
    sem_wait(&semaphore);

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

    sem_post(&semaphore);
  }

  /* CLOSE NCURSES */
  if (endwin() == ERR)
  {
    fprintf(stderr, "ERROR: endwin()");
    exit(ERROR);
  }

  return 0;
}
