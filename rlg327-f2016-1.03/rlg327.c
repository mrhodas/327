#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "dungeon.h"
#include "path.h"
#include "monster.h"
#include "priorityQueue.h"

int main(int argc, char *argv[]) {
    //Main Variables
    dungeon_t d;
    time_t seed;
    int i;

    //Seeding.
    struct timeval tv;
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
    printf("Seed is %ld.\n", seed);
    srand(seed);

    //Create dungeon.
    init_dungeon(&d);
    gen_dungeon(&d);

    //Setup player.
    i = rand() % d.num_rooms;
    d.pc.position[dim_x] = (d.rooms[i].position[dim_x] +
                            (rand() % d.rooms[i].size[dim_x]));
    d.pc.position[dim_y] = (d.rooms[i].position[dim_y] +
                            (rand() % d.rooms[i].size[dim_y]));
    dijkstra(&d);
    dijkstra_tunnel(&d);

    //Setup monsters:
    int monster_num;

    //Check for number of monster flag
    if (argc > 2) {
        if (strcmp(argv[1], "--nummon") == 0) {
            monster_num = atoi(argv[2]);
        }
    } else {
        monster_num = DEFAULT_MONSTER_COUNT;
    }
    //Setup the monster list.
    mon_list.monster_number = monster_num;
    mon_list.monsters = (mon_t *)malloc(sizeof(mon_t) * monster_num);
    mon_list.alive = (int *)malloc(sizeof(int) * monster_num);

    initialize_monsters();

//    for(i = 0; i < mon_list.monster_number; i++){
//        int room_num = rand() % d.num_rooms;
//        mon_list.monsters->position[dim_x] = d.rooms[room_num].position[dim_x];
//        mon_list.monsters->position[dim_y] = d.rooms[room_num].position[dim_y];
//    }

    //Main game loop.
    i = 1;
    int turn = 0;
    while (mon_list.monster_number > 0) {
        render_dungeon(&d);
       // render_monsters(&d);
        sleep(1);

        d.pc.position[dim_y]++;
//        int alive_monsters = num_alive();
//        while (i < alive_monsters) {
//            if (mon_list.alive[i] == 1) {
//                move_monster(i);
//            };
//            i++;
//        }
//
//        turn++;
        mon_list.monster_number--;
    }
    //END OF GAME LOOP!
    printf("\nMonsters left: %d\n", mon_list.monster_number);
    if(mon_list.monster_number == 0) {
        printf("\nYOU WIN!!!\n");
    } else {
        printf("\nYOU LOSE!!!\n");
    }

    //init_queue();


//    insertQ(dummy);
//    insertQ(dummy);
//    printf("\n%d\n", event_queue.size);
//    e_node *n2 = get_next_event();
//    printf("\nEvent #%d prio: %d\n", i, n2->priority);
//
//    printf("\n%d\n", event_queue.size);
//    n2 = get_next_event();
//    printf("\nEvent #%d prio: %d\n", i, n2->priority);

//    printf("\nhere\n");
//    for(i = 0; i < monster_num; i++){
//        insertQ(mon_list.monsters[i]);
//    }
//
//
//    i=0;
//    while(event_queue.size > 0){
//        e_node *n = get_next_event();
//        printf("\nEvent #%d prio: %d\n", i, n->priority);
//        i++;
//    }



    //Clear everything up.
    //free(mon_list);
    delete_dungeon(&d);
    return 0;
}
