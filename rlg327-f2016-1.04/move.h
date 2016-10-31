#ifndef MOVE_H
# define MOVE_H

# include <stdint.h>

# include "dims.h"

typedef struct dungeon dungeon_t;
typedef struct character character_t;
void do_moves(dungeon_t *d, char ch);
void move_character(dungeon_t *d, character_t *c, pair_t next);

#endif
