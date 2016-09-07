#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include "main.h"

char **map;
int rows=21;
int cols=80;



int main(int argc, char** args) {

    map = (char**)malloc(sizeof(char*) * rows);

    for(int i = 0; i < cols; i++){
        map[i] = (char*)malloc(sizeof(char) * cols);
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            map[i][j] = ' ';
        }
    }

    generateDungeon();

    printMap(map);

    return 0;
}

void generateDungeon(){
    int numberOfRooms;

    for(numberOfRooms = 0; numberOfRooms < 30; numberOfRooms++) {

        srand(time(NULL));

        int room_rows = 3 + (rand() % 5);
        int room_cols = 3 + (rand() % 5);

        int row_rand = rand() % (rows - room_rows);
        int col_rand = rand() % (cols - room_cols);

        for (int i = 0; i < room_rows; i++) {
            for (int j = 0; j < room_cols; j++) {
                if(map[row_rand + i][col_rand + j] == '.'){
                    continue;
                }
            }
        }

        for (int i = 0; i < room_rows; i++) {
            for (int j = 0; j < room_cols; j++) {
                map[row_rand + i][col_rand + j] = '.';
            }
        }
    }

}

void addRoom(int x, int y, int width, int height){

}

void printMap(char** map){
    for(int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}