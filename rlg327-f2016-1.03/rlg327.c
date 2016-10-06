#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "dungeon.h"
#include "path.h"
#include "monster.h"

int main(int argc, char *argv[]) {
    dungeon_t d;
    time_t seed;
    struct timeval tv;
    uint32_t i;
    uint32_t do_seed, do_place_pc;

    /* Default behavior: Seed with the time, generate a new dungeon, *
     * and don't write to disk.                                      */
    do_place_pc = 0;
    do_seed = 1;


    //monster number
    int monster_num = 3;

    //check for number of monster flag
    if (argc > 1) {
        if (strcmp(argv[1], "--nummon") == 0) {
            printf("Enter the number of monsters: ");
            scanf("%d", &monster_num);
        }
    }

    printf("\n%d\n", monster_num);

    if (do_seed) {
        /* Allows me to generate more than one dungeon *
         * per second, as opposed to time().           */
        gettimeofday(&tv, NULL);
        seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
    }

    printf("Seed is %ld.\n", seed);
    srand(seed);

    init_dungeon(&d);
    gen_dungeon(&d);

    //Setup player
    if (!do_place_pc) {
        i = rand() % d.num_rooms;
        d.pc.position[dim_x] = (d.rooms[i].position[dim_x] +
                                (rand() % d.rooms[i].size[dim_x]));
        d.pc.position[dim_y] = (d.rooms[i].position[dim_y] +
                                (rand() % d.rooms[i].size[dim_y]));
    }
    dijkstra(&d);
    dijkstra_tunnel(&d);

    render_dungeon(&d);

    //Setup the monster list.
    mon_list.monster_number = monster_num;
    mon_list.monsters = malloc(sizeof(mon_t) * monster_num);
    mon_list.alive = malloc(sizeof(int) * monster_num);

    initialize_monsters();

    while (mon_list.monster_number > 0) {
        sleep(1);
        int alive_monsters = num_alive();





//dead monster
        if (1) {
            mon_list.monster_number--;
        }
    }


    printf("\n%u\n", d.pc.position[dim_x]);

    delete_dungeon(&d);
    return 0;
}
