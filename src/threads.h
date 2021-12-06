#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>
#include <semaphore.h>

pthread_t create_ball_thread(sem_t *semaphore);
void join_ball_thread(pthread_t *thread_id);

#endif