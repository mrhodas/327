#include <math.h>
#include "priorityQueue.h"

void init_queue() {
    event_queue.head = (e_node*)malloc(sizeof(struct e_node));
    event_queue.tail = (e_node*)malloc(sizeof(struct e_node));
    event_queue.size = 0;
    event_queue.head->prev = (e_node*)NULL;
    event_queue.head->next = event_queue.tail;
    event_queue.tail->prev = (e_node*)NULL;
    event_queue.tail->prev = event_queue.head;
}

void insertQ(mon_t monster) {
    int speed;
    struct e_node *n = (e_node *) malloc(sizeof(struct e_node));
    if (monster.speed >= 5) {
        n->monster = monster;
        speed = n->monster.speed;
    } else {
        n->monster = monster;
        speed = PLAYER_SPEED;
    }

    int priority = (TIME_CONSTANT / speed);
    n->priority = priority;

    if (event_queue.size == 0) {
        n->next = event_queue.tail;
        n->prev = event_queue.head;
        event_queue.tail->prev = n;
        event_queue.head->next = n;
    } else {
        struct e_node *temp = event_queue.head->next;

        while (temp->next != NULL) {
            if (n->priority < temp->priority) {
                n->next = temp;
                n->prev = temp->prev;
                temp->prev->next = n;
                temp->prev = n;
                break;
            } else if(n->priority == temp->priority){
                temp = temp->next;
                continue;
            } else if(n->priority > temp->priority){
                n->next = temp;
                n->prev = temp->prev;
                temp->prev->next = n;
                temp->prev = n;
            }
        }
    }
    event_queue.size++;
}

struct e_node* get_next_event(){
    struct e_node *event = event_queue.head->next;
    event->next->prev = event_queue.head;
    event_queue.head->next = event->next;
    event_queue.size--;
    return event;
}