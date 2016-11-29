#ifndef CHARACTER_H
# define CHARACTER_H

# include <stdint.h>
#include <string>
#include <vector>

# include "dims.h"
#include "dice.h"

class character {
public:
    char symbol;
    pair_t position;
    int32_t speed;
    uint32_t alive;
    uint32_t sequence_number;
    std::string name, description;
    uint32_t color;
    int hitpoints;
    dice damage;

    void set(const std::string &name,
        const std::string &description,
        const std::vector<uint32_t> &color,
        const dice &damage,
        const dice &hitpoints);
};

typedef struct dungeon dungeon_t;

int32_t compare_characters_by_next_turn(const void *character1,
                                        const void *character2);

uint32_t can_see(dungeon_t *d, pair_t voyeur, pair_t exhibitionist, int is_pc);

void character_delete(character *c);

int8_t *character_get_pos(character *c);

int8_t character_get_y(const character *c);

int8_t character_set_y(character *c, int8_t y);

int8_t character_get_x(const character *c);

int8_t character_set_x(character *c, int8_t x);

uint32_t character_get_next_turn(const character *c);

void character_die(character *c);

int character_is_alive(const character *c);

void character_next_turn(character *c);

void character_reset_turn(character *c);

char character_get_symbol(const character *c);

uint32_t character_get_speed(const character *c);

#endif
