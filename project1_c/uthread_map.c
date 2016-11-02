//
// Created by Michael on 11/1/2016.
//

#include "uthread_map.h"


int uthread_map_init(umap_t *map) {
    if ((map->next = (umap_t *)malloc(sizeof(umap_t))) == NULL){
        printf("\nError malloc next map element.\n");
        return -1;
    }
    if ((map->id_list = (int *)malloc(sizeof(int))) == NULL){
        printf("\nError malloc map idlist.\n");
        return -1;
    }
    if((map->user_thread_list = (thread_t *)malloc(sizeof(thread_t *))) == NULL){
        printf("\nError malloc map user_thread_list.\n");
        return -1;
    }
    return 0;
}