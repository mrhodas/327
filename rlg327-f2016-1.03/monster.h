#ifndef RLG327_F2016_1_03_MAIN_H
#define RLG327_F2016_1_03_MAIN_H

#include "dungeon.h"

#define HARD_ATTACK    85;

typedef struct monster {
    pair_t position;
    char type;
    int speed;
    int intelligence;
    int telepathy;
    int tunneling;
    int erratic;
    int line_of_sight;
} mon_t;

typedef struct monster_list{
    int monster_number;
    mon_t *monsters;
    int* alive;
} m_list;

m_list mon_list;

double random_frac();
int initialize_monsters();
int num_alive();
#endif



