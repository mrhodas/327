/*
 * Created by Michael Rhodas
 * Com S 352: Project 1
 * uthread library header
*/

#ifndef UTHREAD_H
#define UTHREAD_H

#include <stdio.h>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <queue>
#include <map>

#include "Thread.h"


using namespace std;

class uthread {
private:
    //Global Variables
    int maxThreads;
    int numOfThreads;
    //pthread_mutex_t mapMtx;
    //pthread_mutex_t qMtx;
    //pthread_mutex_t threadMtx;
    map<int, Thread *> *init_thread_map;
    priority_queue<Thread *> *init_thread_pqueue;


public:

    inline void setMaxThreads(int max){
        maxThreads = max;
    }

    uthread(int maxThreads);

    void uthread_init(int numKernelThreads);

    int uthread_create(void (*func)());

    void uthread_yield();

    void uthread_exit();
};

#endif
