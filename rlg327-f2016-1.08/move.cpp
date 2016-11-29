#include "move.h"

#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#include "dungeon.h"
#include "heap.h"
#include "move.h"
#include "npc.h"
#include "pc.h"
#include "character.h"
#include "utils.h"
#include "path.h"
#include "event.h"
#include "io.h"
#include "object.h"

void do_combat(dungeon_t *d, character *atk, character *def) {

    if (atk == d->thepc || def == d->thepc) {
        uint atk_damage = atk->damage->roll();

        if (def->hp <= atk_damage) {
            character_die(def);
            if (def != d->thepc) {
                d->num_monsters--;
            }
            d->charmap[character_get_y(atk)][character_get_x(atk)] = NULL;
            character_set_y(atk, def->position[dim_y]);
            character_set_x(atk, def->position[dim_x]);
            d->charmap[character_get_y(atk)][character_get_x(atk)] = atk;
        } else {
            def->hp -= atk_damage;
        }
    }
}

void move_character(dungeon_t *d, character *c, pair_t next) {
    if (((charpair(next) && next[dim_y] != character_get_y(c)) || (next[dim_x] != character_get_x(c)))) {
        do_combat(d, c, charpair(next));
    } else {
        d->charmap[character_get_y(c)][character_get_x(c)] = NULL;
        character_set_y(c, next[dim_y]);
        character_set_x(c, next[dim_x]);
        d->charmap[character_get_y(c)][character_get_x(c)] = c;


        if (d->thepc == c) {
            if (d->objmap[next[dim_y]][next[dim_x]]) {
                if (d->thepc->inventory.size() < 10) {
                    d->thepc->inventory.push_back(d->objmap[next[dim_y]][next[dim_x]]);
                    d->objmap[next[dim_y]][next[dim_x]] = d->objmap[next[dim_y]][next[dim_x]]->get_next();
                }
            }
        }

    }
    if (c == d->thepc) {
        pc_reset_visibility((pc *) c);
        pc_observe_terrain((pc *) c, d);
    }
}

void do_moves(dungeon_t *d) {
    pair_t next;
    character *c;
    event_t *e;

    /* Remove the PC when it is PC turn.  Replace on next call.  This allows *
     * use to completely uninit the heap when generating a new level without *
     * worrying about deleting the PC.                                       */

    if (pc_is_alive(d)) {
        /* The PC always goes first one a tie, so we don't use new_event().  *
         * We generate one manually so that we can set the PC sequence       *
         * number to zero.                                                   */
        e = (event_t *) malloc(sizeof(*e));
        e->type = event_character_turn;
        /* The next line is buggy.  Monsters get first turn before PC.  *
         * Monster gen code always leaves PC in a monster-free room, so *
         * not a big issue, but it needs a better solution.             */
        /* 1.05: I'm still trying to figure out what the right solution *
         * to this is.  I've come up with several ugly hacks that fix   *
         * it, but there must be a better way...                        */
        e->time = d->time + (1000 / character_get_speed(d->thepc));
        e->sequence = 0;
        e->c = d->thepc;
        heap_insert(&d->events, e);
    }

    while (pc_is_alive(d) &&
           (e = (event_t *) heap_remove_min(&d->events)) &&
           ((e->type != event_character_turn) || (e->c != d->thepc))) {
        d->time = e->time;
        if (e->type == event_character_turn) {
            c = e->c;
        }
        if (!character_is_alive(c)) {
            if (d->charmap[character_get_y(c)][character_get_x(c)] == c) {
                d->charmap[character_get_y(c)][character_get_x(c)] = NULL;
            }
            if (c != d->thepc) {
                event_delete(e);
            }
            continue;
        }

        npc_next_pos(d, (npc *) c, next);
        move_character(d, c, next);

        heap_insert(&d->events, update_event(d, e, 1000 / character_get_speed(c)));
    }

    io_display(d);
    if (pc_is_alive(d) && e->c == d->thepc) {
        c = e->c;
        d->time = e->time;
        /* Kind of kludgey, but because the PC is never in the queue when   *
         * we are outside of this function, the PC event has to get deleted *
         * and recreated every time we leave and re-enter this function.    */
        e->c = NULL;
        event_delete(e);

        io_handle_input(d);
    }
}

void dir_nearest_wall(dungeon_t *d, character *c, pair_t dir) {
    dir[dim_x] = dir[dim_y] = 0;

    if (character_get_x(c) != 1 && character_get_x(c) != DUNGEON_X - 2) {
        dir[dim_x] = (character_get_x(c) > DUNGEON_X - character_get_x(c) ? 1 : -1);
    }
    if (character_get_y(c) != 1 && character_get_y(c) != DUNGEON_Y - 2) {
        dir[dim_y] = (character_get_y(c) > DUNGEON_Y - character_get_y(c) ? 1 : -1);
    }
}

uint32_t in_corner(dungeon_t *d, character *c) {
    uint32_t num_immutable;

    num_immutable = 0;

    num_immutable += (mapxy(character_get_x(c) - 1,
                            character_get_y(c)) == ter_wall_immutable);
    num_immutable += (mapxy(character_get_x(c) + 1,
                            character_get_y(c)) == ter_wall_immutable);
    num_immutable += (mapxy(character_get_x(c),
                            character_get_y(c) - 1) == ter_wall_immutable);
    num_immutable += (mapxy(character_get_x(c),
                            character_get_y(c) + 1) == ter_wall_immutable);

    return num_immutable > 1;
}

static void new_dungeon_level(dungeon_t *d, uint32_t dir) {
    /* Eventually up and down will be independantly meaningful. *
     * For now, simply generate a new dungeon.                  */

    switch (dir) {
        case '<':
        case '>':
            new_dungeon(d);
            break;
        default:
            break;
    }
}

uint32_t move_pc(dungeon_t *d, uint32_t dir) {
    pair_t next;
    uint32_t was_stairs = 0;

    next[dim_y] = character_get_y(d->thepc);
    next[dim_x] = character_get_x(d->thepc);

    switch (dir) {
        case 1:
        case 2:
        case 3:
            next[dim_y]++;
            break;
        case 4:
        case 5:
        case 6:
            break;
        case 7:
        case 8:
        case 9:
            next[dim_y]--;
            break;
    }
    switch (dir) {
        case 1:
        case 4:
        case 7:
            next[dim_x]--;
            break;
        case 2:
        case 5:
        case 8:
            break;
        case 3:
        case 6:
        case 9:
            next[dim_x]++;
            break;
        case '<':
            if (mappair(character_get_pos(d->thepc)) == ter_stairs_up) {
                was_stairs = 1;
                new_dungeon_level(d, '<');
            }
            break;
        case '>':
            if (mappair(character_get_pos(d->thepc)) == ter_stairs_down) {
                was_stairs = 1;
                new_dungeon_level(d, '>');
            }
            break;
    }

    if (was_stairs) {
        return 0;
    }

    if ((dir != '>') && (dir != '<') && (mappair(next) >= ter_floor)) {
        move_character(d, d->thepc, next);
        dijkstra(d);
        dijkstra_tunnel(d);

        return 0;
    }

    return 1;
}
