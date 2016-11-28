#include <stdlib.h>

#include "string.h"

#include "dungeon.h"
#include "Pc.h"
#include "Character.h"
#include "utils.h"
#include "move.h"
#include "path.h"

#ifdef __cplusplus
extern "C" {
#endif

void pc_delete(pc_t *pc) {
    if (pc) {
        free(pc);
    }
}

uint32_t pc_is_alive(dungeon_t *d) {
    return d->pc.alive;
}

void place_pc(dungeon_t *d) {
    d->pc.position[dim_y] = rand_range(d->rooms->position[dim_y],
                                       (d->rooms->position[dim_y] +
                                        d->rooms->size[dim_y] - 1));
    d->pc.position[dim_x] = rand_range(d->rooms->position[dim_x],
                                       (d->rooms->position[dim_x] +
                                        d->rooms->size[dim_x] - 1));
}

void config_pc(dungeon_t *d) {
    memset(&d->pc, 0, sizeof(d->pc));
    d->pc.symbol = '@';

    place_pc(d);

    d->pc.speed = PC_SPEED;
    d->pc.alive = 1;
    d->pc.sequence_number = 0;
    d->pc.pc = (Pc *)calloc(1, sizeof(*d->pc.pc));
    d->pc.npc = NULL;

    d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = &d->pc;

    dijkstra(d);
    dijkstra_tunnel(d);

    init_seen(d);
    //update_seen(d);
}

void init_seen(dungeon_t *d) {
    int x, y;

    for (y = 0; y < DUNGEON_Y; y++) {
        for (x = 0; x < DUNGEON_X; x++) {
            seenxy(x, y) = 0;
            seenmapxy(x, y) = ter_unknown;
        }
    }
    update_surroundings(d);
}

void update_surroundings(dungeon_t *d){
    int y, x;
    for (y = d->pc.position[dim_y] - 3; y <= d->pc.position[dim_y] + 3; y++) {
        for (x = d->pc.position[dim_x] - 3; x <= d->pc.position[dim_x] + 3; x++) {
            if(y < 0 || y >= DUNGEON_Y){
                break;
            }
            if(x < 0 || x >= DUNGEON_X){
                continue;
            }
            seenmapxy(x, y) = mapxy(x, y);
            seenxy(x, y) = 1;
        }
    }
}

/* Not used anymore, but leaving it here because you could *
 * adapt it if you wanted to implement a bot player.       */
uint32_t pc_next_pos(dungeon_t *d, pair_t dir) {
    dir[dim_y] = dir[dim_x] = 0;

    /* Tunnel to the nearest dungeon corner, then move around in hopes *
     * of killing a couple of monsters before we die ourself.          */

    if (in_corner(d, &d->pc)) {
        /*
        dir[dim_x] = (mapxy(d->pc.position[dim_x] - 1,
                            d->pc.position[dim_y]) ==
                      ter_wall_immutable) ? 1 : -1;
        */
        dir[dim_y] = (mapxy(d->pc.position[dim_x],
                            d->pc.position[dim_y] - 1) ==
                      ter_wall_immutable) ? 1 : -1;
    } else {
        dir_nearest_wall(d, &d->pc, dir);
    }

    return 0;
}

#ifdef __cplusplus
}
#endif