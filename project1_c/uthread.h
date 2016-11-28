/*
 * Created by Michael Rhodas
 * Com S 352: Project 1
 * uthread library header
 * most help gotten from class notes, stack overflow, and files from Com S 327.
*/

#ifndef UTHREAD_H
#define UTHREAD_H

#define STACK_MAX   16384

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "thread.h"
#include "uthread_map.h"

typedef struct user_thread_map umap_t;
typedef struct user_thread_queue uqueue_t;

typedef struct uthread {
    int max_threads;
    int num_of_threads;
    pthread_mutex_t mapMtx;
    pthread_mutex_t queueMtx;
    pthread_mutex_t threadMtx;
    umap_t *map;
    int map_size;
} uthread_t;

uthread_t u;

void uthread_init(int numKernelThreads);

int uthread_create(void (*func)());

void uthread_yield();

void uthread_exit();

#endif
