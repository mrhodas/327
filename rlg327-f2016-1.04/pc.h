#ifndef PC_H
# define PC_H

# include <stdint.h>

# include "dims.h"

typedef struct dungeon dungeon_t;

typedef struct pc {
} pc_t;

void pc_delete(pc_t *pc);
uint32_t pc_is_alive(dungeon_t *d);
void config_pc(dungeon_t *d);
uint32_t pc_next_pos(dungeon_t *d, pair_t dir, char ch);
void place_pc(dungeon_t *d);

#endif
