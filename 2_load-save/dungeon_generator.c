//
// Created by Michael on 9/7/2016.
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "main.h"

char **map;
int rows = 21;
int cols = 80;

int main(int argc, char **args) {
    int i, j;

    map = (char **) malloc(sizeof(char *) * rows);

    for (i = 0; i < rows; i++) {
        map[i] = (char *) malloc(sizeof(char) * cols);
        for (j = 0; j < cols; j++) {
            map[i][j] = ' ';
        }
    }

    generateDungeon();
    printMap(map);

    return 0;
}

void generateDungeon() {
    srand(time(NULL));
    int maxRooms = 6 + (rand() % 4);
    int *roomList = (int *) calloc(maxRooms * 4, sizeof(int));

    int numberOfRooms = 0;
    while (numberOfRooms < maxRooms) {
        int room_width = 4 + (rand() % 5);
        int room_height = 3 + (rand() % 5);

        int row_pos_rand = (rand() % (rows - room_width - 2)) + 1;
        int col_pos_rand = (rand() % (cols - room_height - 2)) + 1;

        if (checkPlacement(row_pos_rand, col_pos_rand, room_width, room_height)) {
            addRoom(row_pos_rand, col_pos_rand, room_width, room_height);
            roomList[numberOfRooms * 4] = row_pos_rand;
            roomList[numberOfRooms * 4 + 1] = col_pos_rand;
            roomList[numberOfRooms * 4 + 2] = room_width;
            roomList[numberOfRooms * 4 + 3] = room_height;
            numberOfRooms++;
            srand(time(NULL));
        }
    }

    drawCorridors(roomList, numberOfRooms);
}

void drawCorridors(int *roomList, int numberOfRooms) {
    int currentRoom = 0;

    while (currentRoom < numberOfRooms - 1) {
        int currentRow = roomList[currentRoom * 4];
        int currentCol = roomList[currentRoom * 4 + 1];
        int currentWidth = roomList[currentRoom * 4 + 2];
        int currentHeight = roomList[currentRoom * 4 + 3];

        int nextRow = roomList[(currentRoom + 1) * 4];
        int nextCol = roomList[(currentRoom + 1) * 4 + 1];
        int nextWidth = roomList[(currentRoom + 1) * 4 + 2];
        int nextHeight = roomList[(currentRoom + 1) * 4 + 3];

        if (currentCol < nextCol) {
            for (currentCol; currentCol < nextCol; currentCol++) {
                if (map[currentRow][currentCol] == ' ') {
                    map[currentRow][currentCol] = '#';
                }
            }

            //current is top-left
            if (currentRow < nextRow) {
                for (currentRow; currentRow < nextRow; currentRow++) {
                    if (map[currentRow][nextCol] == ' ') {
                        map[currentRow][nextCol] = '#';
                    }
                }
            }
                //current is bottom-left
            else {
                for (currentRow; currentRow > nextRow; currentRow--) {
                    if (map[currentRow][nextCol] == ' ') {
                        map[currentRow][nextCol] = '#';
                    }
                }
            }
        } else {
            for (currentCol; currentCol > nextCol; currentCol--) {
                if (map[currentRow][currentCol] == ' ') {
                    map[currentRow][currentCol] = '#';
                }
            }

            //current is top-right
            if (currentRow < nextRow) {
                for (nextRow; nextRow >= currentRow; nextRow--) {
                    if (map[nextRow][nextCol] == ' ') {
                        map[nextRow][nextCol] = '#';
                    }
                }
            }
                //current is bottom-right
            else {
                for (currentRow; currentRow > nextRow; currentRow--) {
                    if (map[currentRow][nextCol] == ' ') {
                        map[currentRow][nextCol] = '#';
                    }
                }
            }
        }
        currentRoom++;
    }
}

int checkPlacement(int x, int y, int width, int height) {
    int i, j;
    for (i = -1; i < width + 1; i++) {
        for (j = -1; j < height + 1; j++) {
            if (map[x + i][y + j] == '.') {
                return 0;
            }
        }
    }
    return 1;
}

void addRoom(int x, int y, int width, int height) {
    int i, j;
    for (i = 0; i < width; i++) {
        for (j = 0; j < height; j++) {
            map[x + i][y + j] = '.';
        }
    }
}

void printMap(char **map) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
}