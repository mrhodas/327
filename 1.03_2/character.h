#ifndef CHARACTER_H
#define CHARACTER_H

#include "core.h"

typedef struct
{
	// char representation of this character
	char c;
	unsigned char x, y;

	// TODO add more fields here for 1.04

} Character;

extern Character *pc;

/* character map */
Character *cmap[DUNG_H][DUNG_W];

/* create a character */
Character *character_new(char c);

/* delete a character */
int character_delete(Character*);

/* randomly place the character in the dungeon */
int character_place(Character*);

#endif

