#include "Character.h"

using namespace std;

Character::Character(int x, int y){
    //Initiate Position
    pair_t pos;
    pos[dim_x] = x;
    pos[dim_y] = y;
    position = pos;

    //Start alive
    alive = 1;

}

Character::~Character(){

}

pair_t Character::get_position_character(){
    return position;
}

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "heap.h"
#include "Npc.h"
#include "Pc.h"
#include "dungeon.h"

pair_t cget_position_character(struct Character * c){
    return c->get_position_character();
}

//Npc get_npc(){
//    return cget
//}

void character_delete(Character *v) {
    /* The PC is never malloc()ed anymore, do don't attempt to free it here. */
    Character *c;

    if (v) {
        c = v;

        if (c->npc) {
            //npc_delete(c->npc);
            free(c);
        }
    }
}

uint32_t can_see(dungeon_t *d, Character *voyeur, Character *exhibitionist) {
    /* Application of Bresenham's Line Drawing Algorithm.  If we can draw *
     * a line from v to e without intersecting any walls, then v can see  *
     * e.  Unfortunately, Bresenham isn't symmetric, so line-of-sight     *
     * based on this approach is not reciprocal (Helmholtz Reciprocity).  *
     * This is a very real problem in roguelike games, and one we're      *
     * going to ignore for now.  Algorithms that are symmetrical are far  *
     * more expensive.                                                    */

    pair_t first, second;
    pair_t del, f;
    int16_t a, b, c, i;

    first[dim_x] = (voyeur->position)[dim_x];
    first[dim_y] = voyeur->position[dim_y];
    second[dim_x] = exhibitionist->position[dim_x];
    second[dim_y] = exhibitionist->position[dim_y];

    if ((abs(first[dim_x] - second[dim_x]) > VISUAL_RANGE) ||
        (abs(first[dim_y] - second[dim_y]) > VISUAL_RANGE)) {
        return 0;
    }

    /*
    mappair(first) = ter_debug;
    mappair(second) = ter_debug;
    */

    if (second[dim_x] > first[dim_x]) {
        del[dim_x] = second[dim_x] - first[dim_x];
        f[dim_x] = 1;
    } else {
        del[dim_x] = first[dim_x] - second[dim_x];
        f[dim_x] = -1;
    }

    if (second[dim_y] > first[dim_y]) {
        del[dim_y] = second[dim_y] - first[dim_y];
        f[dim_y] = 1;
    } else {
        del[dim_y] = first[dim_y] - second[dim_y];
        f[dim_y] = -1;
    }

    if (del[dim_x] > del[dim_y]) {
        a = del[dim_y] + del[dim_y];
        c = a - del[dim_x];
        b = c - del[dim_x];
        for (i = 0; i <= del[dim_x]; i++) {
            if ((mappair(first) < ter_floor) && i && (i != del[dim_x])) {
                return 0;
            }
            /*      mappair(first) = ter_debug;*/
            first[dim_x] += f[dim_x];
            if (c < 0) {
                c += a;
            } else {
                c += b;
                first[dim_y] += f[dim_y];
            }
        }
        return 1;
    } else {
        a = del[dim_x] + del[dim_x];
        c = a - del[dim_y];
        b = c - del[dim_y];
        for (i = 0; i <= del[dim_y]; i++) {
            if ((mappair(first) < ter_floor) && i && (i != del[dim_y])) {
                return 0;
            }
            /*      mappair(first) = ter_debug;*/
            first[dim_y] += f[dim_y];
            if (c < 0) {
                c += a;
            } else {
                c += b;
                first[dim_x] += f[dim_x];
            }
        }
        return 1;
    }
    return 1;
}

#ifdef __cplusplus
}
#endif
