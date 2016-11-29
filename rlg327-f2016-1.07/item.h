//
// Created by Michael on 11/9/2016.
//

#ifndef RLG327_F2016_1_07_ITEM_H
#define RLG327_F2016_1_07_ITEM_H


#include "character.h"
#include "descriptions.h"

class item {
public:
    char symbol;
    pair_t position;
    std::string name, description;
    object_type_t type;
    uint32_t color;
    int hit_bonus;
    int damage_bonus;
    int dodge_bonus;
    int weight;
    int speed_bonus;
    int special;
    dice val;
};

char item_get_symbol(const item *c);

#endif //RLG327_F2016_1_07_ITEM_H
