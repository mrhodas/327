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

void do_combat(dungeon_t *d, character_t *atk, character_t *def) {
    def->alive = 0;
    if (def != &d->pc) {
        d->num_monsters--;
    }
}

void move_character(dungeon_t *d, character_t *c, pair_t next) {
    if (charpair(next) &&
        ((next[dim_y] != c->position[dim_y]) ||
         (next[dim_x] != c->position[dim_x]))) {
        do_combat(d, c, charpair(next));
    } else {
        /* No character in new position. */

        d->character[c->position[dim_y]][c->position[dim_x]] = NULL;
        c->position[dim_y] = next[dim_y];
        c->position[dim_x] = next[dim_x];
        d->character[c->position[dim_y]][c->position[dim_x]] = c;
    }
}

void do_moves(dungeon_t *d, char in) {
    pair_t next;
    next[dim_x] = next[dim_y] = 0;
    character_t *c;
    event_t *e;

    /* Remove the PC when it is PC turn.  Replace on next call.  This allows *
     * use to completely uninit the heap when generating a new level without *
     * worrying about deleting the PC.                                       */

    if (pc_is_alive(d)) {
        /* The PC always goes first one a tie, so we don't use new_event().  *
         * We generate one manually so that we can set the PC sequence       *
         * number to zero.                                                   */
        e = malloc(sizeof(*e));
        e->type = event_character_turn;
        /* The next line is buggy.  Monsters get first turn before PC.  *
         * Monster gen code always leaves PC in a monster-free room, so *
         * not a big issue, but it needs a better solution.             */
        e->time = d->time + (1000 / d->pc.speed);
        e->sequence = 0;
        e->c = &d->pc;
        heap_insert(&d->events, e);
    }

    while (pc_is_alive(d) &&
           (e = heap_remove_min(&d->events)) &&
           ((e->type != event_character_turn) || (e->c != &d->pc))) {
        d->time = e->time;
        if (e->type == event_character_turn) {
            c = e->c;
        }
        if (!c->alive) {
            if (d->character[c->position[dim_y]][c->position[dim_x]] == c) {
                d->character[c->position[dim_y]][c->position[dim_x]] = NULL;
            }
            if (c != &d->pc) {
                event_delete(e);
            }
            continue;
        }

        npc_next_pos(d, c, next);
        if (e->c != &d->pc) {
            move_character(d, c, next);
        }
        heap_insert(&d->events, update_event(d, e, 1000 / c->speed));
    }

    if (pc_is_alive(d) && e->c == &d->pc) {
        c = e->c;
        d->time = e->time;
        /* Kind of kludgey, but because the PC is never in the queue when   *
         * we are outside of this function, the PC event has to get deleted *
         * and recreated every time we leave and re-enter this function.    */
        e->c = NULL;
        event_delete(e);
        pc_next_pos(d, next, in);
        next[dim_x] += c->position[dim_x];
        next[dim_y] += c->position[dim_y];


        if (mappair(next) <= ter_floor) {
            mappair(next) = ter_floor_hall;
        }


        move_character(d, c, next);

        dijkstra(d);
        dijkstra_tunnel(d);
    }
}
