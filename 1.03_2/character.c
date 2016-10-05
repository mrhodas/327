#include "core.h"
#include "character.h"
#include "dungeon.h"

Character *character_new(char ch)
{
	Character *c = malloc(sizeof(*c));
	c->c = ch;
	return c;
}

int character_delete(Character *c)
{
	free(c);
	return 0;
}

int character_place(Character *c)
{
	while(1)
	{
		int x = rand() % DUNG_W;
		int y = rand() % DUNG_H;

		if (tmap[y][x]==ROOM)
		{
			cmap[y][x] = c;
			
			c->x = x;
			c->y = y;
			break;
		}
	}
	return 0;
}

