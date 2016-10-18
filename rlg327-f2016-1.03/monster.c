#include "monster.h"
#include "dungeon.h"

// returns a double between 0 and 1.
double random_frac() {
    return (double) rand() / (double) RAND_MAX;
}

//initialize the monster array
int initialize_monsters() {

    int i = 0;
    while (mon_list.monster_number > i) {
        //Allocate type memory.
        mon_list.monsters[i].type = (char*)malloc(sizeof(char) * NUM_OF_TYPES);
        float probability = random_frac();
        if (probability <= .5) {
            mon_list.monsters[i].type[INTELLIGENCE] = 1;
            mon_list.monsters[i].representation = 'I';
        } else {
            mon_list.monsters[i].type[INTELLIGENCE] = 0;
            mon_list.monsters[i].representation = 'J';

        }
        probability = random_frac();
        if (probability <= .5) {
            mon_list.monsters[i].type[TELEPATHY] = 1;
            mon_list.monsters[i].representation = 'T';

        } else {
            mon_list.monsters[i].type[TELEPATHY] = 0;
            mon_list.monsters[i].representation = 'K';

        }
        probability = random_frac();
        if (probability <= .5) {
            mon_list.monsters[i].type[TUNNELING] = 1;
            mon_list.monsters[i].representation = 'U';

        } else {
            mon_list.monsters[i].type[TUNNELING] = 0;
            mon_list.monsters[i].representation = 'L';

        }
        probability = random_frac();
        if (probability <= .5) {
            mon_list.monsters[i].type[ERRATIC] = 1;
            mon_list.monsters[i].representation = 'E';

        } else {
            mon_list.monsters[i].type[ERRATIC] = 0;
            mon_list.monsters[i].representation = 'S';

        }

        //Make them all alive.
        mon_list.alive[i] = 1;

        //Set their speed.
        mon_list.monsters[i].speed = 5 + (rand() % 16);
        i++;

        ////TODO
        //mon_list.monsters[i].representation = type_monster(i);
    }

    return 0;
}

char type_monster(int i) {
    char t = ' ';

    //monster is INT
    if (mon_list.monsters[i].type[INTELLIGENCE] == 1) {
        //INT and TELE
        if (mon_list.monsters[i].type[TELEPATHY] == 1) {
            //INT and TELE and TUNNEL
            if (mon_list.monsters[i].type[TUNNELING] == 1) {
                //INT and TELE and TUNNEL and ERRATIC
                if (mon_list.monsters[i].type[ERRATIC] == 1) {

                }
                    //INT and TELE and TUNNEL, no ERRATIC
                else {

                }
            }
                //INT and TELE, no TUNNEL
            else {

            }

        }
            //INT AND NOT TELE
        else {

        }

    }
        //not INTELLIDENT
    else {
        //TELE, no INT
        if (mon_list.monsters[i].type[TELEPATHY] == 1) {
            //TELE and TUNNEL, no INT
            if (mon_list.monsters[i].type[TUNNELING] == 1) {
                //TELE and TUNNEL and ERRATIC, no INT
                if (mon_list.monsters[i].type[ERRATIC] == 1) {

                }
                    //TELE and TUNNEL, no ERRATIC, no INT
                else {

                }
            }
                //TELE, no TUNNEL, no INT
            else {

            }

        }
            //TELE, no INT
        else {

        }


    }


    return t;
}

//Put Monsters on map
//void render_monsters(dungeon_t *d){
//    int i;
//    for(i = 0; i < mon_list.monster_number; i++){
//        d->map[mon_list.monsters[i].position[dim_x]][mon_list.monsters[i].position[dim_y]] = monster;
//    }
//}

//Returns current number of monsters alive.
int num_alive() {
    int alive, i;
    for (i = 0; i < mon_list.monster_number; i++) {
        if (mon_list.alive[i] == 1) {
            alive++;
        }
    }
    return alive;
}

//Moves the monster.
int move_monster(int i) {


    return 0;
}