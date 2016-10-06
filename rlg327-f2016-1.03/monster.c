#include "monster.h"

// returns a double between 0 and 1.
double random_frac() {
    return (double) rand() / (double) RAND_MAX;
}

//initialize the monster array
int initialize_monsters() {
    int i = 0;
    while (mon_list.monster_number > i) {
        double probability = random_frac();
        if (probability <= .5) {
            mon_list.monsters[i].intelligence = 1;
        } else {
            mon_list.monsters[i].intelligence = 0;
        }
        if (probability <= .5) {
            mon_list.monsters[i].telepathy = 1;
        } else {
            mon_list.monsters[i].telepathy = 0;
        }
        if (probability <= .5) {
            mon_list.monsters[i].tunneling = 1;
        } else {
            mon_list.monsters[i].tunneling = 0;
        }
        if (probability <= .5) {
            mon_list.monsters[i].erratic = 1;
        } else {
            mon_list.monsters[i].erratic = 0;
        }
        //Make them all alive.
        mon_list.alive[i] = 1;

        //Set their speed.
        mon_list.monsters->speed = 5 + (rand() % 16);
        i++;
    }
}

//Returns current number of monsters alive.
int num_alive(){
    int alive, i;
    for(i = 0; i < mon_list.monster_number; i++){
        if(mon_list.alive[i] == 1){
            alive++;
        }
    }
    return alive;
}