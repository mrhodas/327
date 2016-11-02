/*
 * Created by Michael Rhodas
 * Com S 352: Project 1
 * 10/31/2016
*/

#include "uthread.h"

int foo(){
    int pid = getpid();
    printf("\nPID: %d\n", pid);
}

int main(int argc, char *argv[]) {
    printf("\n---Start---\n");

    //uthread_init(10);


    int i;
    int tid;
    for(i=0;i<10;i++) {
        void*p;
        p=(void	*)malloc(16384);
        p+=16383;
        if ((tid = clone(foo, p, NULL, 0)) == -1) {
            return -1;
        }
        printf("\nTID: %d\n", tid);
    }

    return 0;
}

