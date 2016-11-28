#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "dungeon.h"
#include "monster.h"

#define TIME_CONSTANT 100

//Make an even node to represent a certain event.
typedef struct e_node {
    int priority;
    mon_t monster;
    struct e_node *next;
    struct e_node *prev;
} e_node;

//Make a priority queue structure to use for event handling.
typedef struct p_queue {
    int size;
    struct e_node *head;
    struct e_node *tail;
} p_queue;

struct p_queue event_queue;

void init_queue();
void insertQ(mon_t monster);
struct e_node* get_next_event();

#endif
