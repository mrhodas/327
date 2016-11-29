#ifndef PC_H
# define PC_H

# include <stdint.h>

# include "dims.h"
# include "character.h"
# include "dungeon.h"

class pc : public character {
public:
    terrain_type_t known_terrain[DUNGEON_Y][DUNGEON_X];
    unsigned char visible[DUNGEON_Y][DUNGEON_X];

    std::vector<object*> equipment;
    std::vector<object*> inventory;

    int damage_bonus();
    void update_speed_bonus();
    int remove(uint index);
    int equip(uint inv);
    int expunge(uint inv);
    int drop(uint inv, dungeon_t *d);
    int inspect(uint inv);
};

class object;

typedef struct dungeon dungeon_t;

typedef enum equip_slot {
    WEAPON,
    OFFHAND,
    RANGED,
    ARMOR,
    HELMET,
    CLOAK,
    GLOVES,
    BOOTS,
    AMULET,
    LIGHT,
    RING1,
    RING2
} slot_t;


#include "dungeon.h"

uint32_t pc_is_alive(dungeon_t *d);

void pc_config(dungeon_t *d);

uint32_t pc_next_pos(dungeon_t *d, pair_t dir);

void place_pc(dungeon_t *d);

void pc_learn_terrain(pc *p, pair_t pos, terrain_type_t ter);

terrain_type_t pc_learned_terrain(pc *p, int8_t y, int8_t x);

void pc_init_known_terrain(pc *p);

void pc_observe_terrain(pc *p, dungeon_t *d);

int32_t is_illuminated(pc *p, int8_t y, int8_t x);

void pc_reset_visibility(pc *p);

void pc_see_object(character *the_pc, object *o);

#endif
