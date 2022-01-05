#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>
#include <semaphore.h>

void *ball_thread_routine(void *args);
void *opponent_thread_routine(void *args);

pthread_t create_ball_thread(sem_t *semaphore);
pthread_t create_opponent_thread(sem_t *semaphore);

void join_ball_thread(pthread_t thread_id);
void join_opponent_thread(pthread_t thread_id);

#endif