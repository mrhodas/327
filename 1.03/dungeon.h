#ifndef DUNGEON_H
#define DUNGEON_H

/* terrain */
char tmap[DUNG_H][DUNG_W];

/* hardness */
unsigned char hmap[DUNG_H][DUNG_W];

/* distance map */
char dmap[DUNG_H][DUNG_W];

/* player x position */
int p_row;

/* player y position */
int p_col;

/* print dungeon */
int dungeon_print();

/* generate random dungeon */
int dungeon_generate();

/* load dungeon using --load <file> */
int dungeon_load();

/* save dungeon using --save <file> */
int dungeon_save();

#endif

