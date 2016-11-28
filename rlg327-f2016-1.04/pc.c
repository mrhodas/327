#include <stdlib.h>

#include "string.h"

#include "dungeon.h"
#include "pc.h"
#include "utils.h"
#include "move.h"
#include "path.h"

<<<<<<< HEAD
void pc_delete(pc_t *pc)
{
  if (pc) {
    free(pc);
  }
}

uint32_t pc_is_alive(dungeon_t *d)
{
  return d->pc.alive;
}

void place_pc(dungeon_t *d)
{
  d->pc.position[dim_y] = rand_range(d->rooms->position[dim_y],
                                     (d->rooms->position[dim_y] +
                                      d->rooms->size[dim_y] - 1));
  d->pc.position[dim_x] = rand_range(d->rooms->position[dim_x],
                                     (d->rooms->position[dim_x] +
                                      d->rooms->size[dim_x] - 1));
}

void config_pc(dungeon_t *d)
{
  memset(&d->pc, 0, sizeof (d->pc));
  d->pc.symbol = '@';

  place_pc(d);

  d->pc.speed = PC_SPEED;
  d->pc.alive = 1;
  d->pc.sequence_number = 0;
  d->pc.pc = calloc(1, sizeof (*d->pc.pc));
  d->pc.npc = NULL;

  d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = &d->pc;

  dijkstra(d);
  dijkstra_tunnel(d);
}

uint32_t pc_next_pos(dungeon_t *d, pair_t dir)
{
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
=======
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
    d->pc.pc = calloc(1, sizeof(*d->pc.pc));
    d->pc.npc = NULL;

    d->character[d->pc.position[dim_y]][d->pc.position[dim_x]] = &d->pc;

    dijkstra(d);
    dijkstra_tunnel(d);
}

uint32_t pc_next_pos(dungeon_t *d, pair_t dir, char ch) {
    dir[dim_y] = dir[dim_x] = 0;

    if (ch == '8' || ch == 'k') {
        dir[dim_y] = -1;
    } else if (ch == '7' || ch == 'y') {
        dir[dim_y] = -1;
        dir[dim_x] = -1;
    } else if (ch == '9' || ch == 'u') {
        dir[dim_y] = -1;
        dir[dim_x] = 1;
    } else if (ch == '6' || ch == 'l') {
        dir[dim_x] = 1;
    } else if (ch == '3' || ch == 'n') {
        dir[dim_y] = 1;
        dir[dim_x] = 1;
    } else if (ch == '2' || ch == 'j') {
        dir[dim_y] = 1;
    } else if (ch == '1' || ch == 'b') {
        dir[dim_y] = 1;
        dir[dim_x] = -1;
    } else if (ch == '4' || ch == 'h') {
        dir[dim_x] = -1;
    } else if (ch == '5' || ch == ' ') {

    }

    if (mapxy(d->pc.position[dim_x] + dir[dim_x],
              d->pc.position[dim_y] + dir[dim_y]) <= ter_wall_immutable) {
        dir[dim_y] = dir[dim_x] = 0;
    }

    return 0;
>>>>>>> afaa278bbb54864f37bfd875d2473e3eeeddab5d
}
