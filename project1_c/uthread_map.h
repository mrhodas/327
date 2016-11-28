//
// Created by Michael on 11/1/2016.
//

#ifndef UTHREAD_MAP_H
#define UTHREAD_MAP_H

#include "uthread.h"

typedef struct user_thread_map umap_t;

typedef struct user_thread_map {
    umap_t *next;
    int *id_list;
    thread_t *user_thread_list;
} umap_t;

int uthread_map_init(umap_t *map);

int umap_insert(umap_t *map, int );

#endif
