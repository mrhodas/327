#ifndef PC_H
# define PC_H

# include <stdint.h>

# include "Character.h"
# include "dims.h"


#ifdef __cplusplus

# include <iostream>

using namespace std;

class Pc : public Character {
public:
    int armor;
    Pc(int x, int y);
    ~Pc();
};

#endif

typedef void pc_t;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dungeon dungeon_t;

void pc_delete(pc_t *pc);

uint32_t pc_is_alive(dungeon_t *d);

void config_pc(dungeon_t *d);

uint32_t pc_next_pos(dungeon_t *d, pair_t dir);

void place_pc(dungeon_t *d);

//void update_seen(dungeon_t *d);

void init_seen(dungeon_t *d);

void update_surroundings(dungeon_t *d);

#ifdef __cplusplus
}
#endif

#endif
