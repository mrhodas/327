#include "monster.h"

// returns a double between 0 and 1.
double random_frac() {
    return (double) rand() / (double) RAND_MAX;
}

//initialize the monster array
int initialize_monsters() {

    int i = 0;
    while (mon_list.monster_number > i) {
        //Allocate type memory.
        mon_list.monsters[i].type = malloc(sizeof(int) * NUM_OF_TYPES);
        float probability = random_frac();
        if (probability <= .5) {
            mon_list.monsters[i].type[INTELLIGENCE] = 1;
        } else {
            mon_list.monsters[i].type[INTELLIGENCE] = 0;
        }
        probability = random_frac();
        if (probability <= .5) {
            mon_list.monsters[i].type[TELEPATHY] = 1;
        } else {
            mon_list.monsters[i].type[TELEPATHY] = 0;
        }
        probability = random_frac();
        if (probability <= .5) {
            mon_list.monsters[i].type[TUNNELING] = 1;
        } else {
            mon_list.monsters[i].type[TUNNELING] = 0;
        }
        probability = random_frac();
        if (probability <= .5) {
            mon_list.monsters[i].type[ERRATIC] = 1;
        } else {
            mon_list.monsters[i].type[ERRATIC] = 0;
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