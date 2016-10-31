#ifndef CHARACTER_H
# define CHARACTER_H

# include <stdint.h>
# include "dims.h"
# include "Npc.h"
# include "Pc.h"

#ifdef __cplusplus

# include <iostream>

using namespace std;

class Character {
public:
    char symbol;
    pair_t position;
    int32_t speed;
    uint32_t alive;
    uint32_t sequence_number;
    Npc *npc;
    Pc *pc;

    Character(int x, int y);
    ~Character();

    inline char get_symbol() {
        return symbol;
    }
    inline pair_t get_pair() {
        return position;
    }
    inline int get_pair_x() {
        return position[dim_x];
    }
    inline int get_pair_y() {
        return position[dim_y];
    }
//    inline Npc get_npc() {
//        return Npc;
//    }

    pair_t get_position_character();

};

#endif
typedef struct character_t;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dungeon dungeon_t;
typedef struct dice_t dice_t;

//typedef struct Character {
//    char symbol;
//    pair_t position;
//    int32_t speed;
//    uint32_t alive;
//    /* Characters use to have a next_turn for the move queue.  Now that it is *
//     * an event queue, there's no need for that here.  Instead it's in the    *
//     * event.  Similarly, sequence_number was introduced in order to ensure   *
//     * that the queue remains stable.  Also no longer necessary here, but in  *
//     * this case, we'll keep it, because it provides a bit of interesting     *
//     * metadata: locally, how old is this Character; and globally, how many   *
//     * characters have been created by the game.                              */
//    uint32_t sequence_number;
//    npc_t *npc;
//    pc_t *pc;
//} character_t;

int32_t compare_characters_by_next_turn(const void *character1,
                                        const void *character2);

uint32_t can_see(dungeon_t *d, character_t *voyeur, character_t *exhibitionist);

void character_delete(void *c);

pair_t cget_position_character(Character);

int8_t cget_position_x(Character);

int8_t cget_position_y(Character);

Npc get_npc(Character);

#ifdef __cplusplus
}
#endif

#endif
