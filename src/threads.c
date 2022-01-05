#include <threads.h>
#include <semaphore.h>
#include <pthread.h>
#include <game_config.h>
#include <ncurses.h>
#include <stdint.h>
#include <player.h>
#include <ball.h>


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