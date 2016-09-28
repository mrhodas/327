//
// Created by Michael Rhodas on 9/6/16.
//

#ifndef ASSIGNMENT1_MAIN_H
#define ASSIGNMENT1_MAIN_H

#endif //ASSIGNMENT1_MAIN_H

void printMap(char** map);
void generateDungeon();
void addRoom(int x, int y, int width, int height);
int checkPlacement(int x, int y, int width, int height);
void drawCorridors(int* roomList, int numberOfRooms);
int save();
int load();
