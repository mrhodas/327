//
// Created by Michael on 11/1/2016.
//

#include "uthread.h"

void uthread_init(int numKernelThreads){
    u.max_threads = numKernelThreads;
    u.num_of_threads = 1;
    uthread_map_init(u.map);
    u.map_size = 1;
}

int uthread_create(void (*func)()){
    return 0;
}

void uthread_yield(){

}

void uthread_exit(){

}