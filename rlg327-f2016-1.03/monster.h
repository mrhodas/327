#ifndef MONSTER_H
#define MONSTER_H

#include "dungeon.h"

#define HARD_ATTACK    85;
#define DEFAULT_MONSTER_COUNT 3;

typedef struct monster {
    pair_t position;
    char representation;
    char* type;
    int speed;
    int line_of_sight;
} mon_t;

typedef struct monster_list{
    int monster_number;
    mon_t *monsters;
    int* alive;
} m_list;

//Global monster list.
m_list mon_list;

double random_frac();
int initialize_monsters();
int num_alive();
int move_monster(int i);
//void render_monsters(dungeon_t *d);
char type_monster(int i);
#endif



