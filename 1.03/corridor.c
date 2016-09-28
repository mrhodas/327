#include "core.h"
#include "dungeon.h"
#include "room.h"

static int numAdjacentCorridors(int r, int c)
{
	int num = 0;
	if(tmap[r-1][c]==CORR)
		num++;
	if(tmap[r+1][c]==CORR)
		num++;
	if(tmap[r][c-1]==CORR)
		num++;
	if(tmap[r][c+1]==CORR)
		num++;
	return num;
}

void path_pave(Room from, Room to)
{
	int fromx, fromy, tox, toy;

	room_centroid(
			&fromx, 
			&fromy,
			from);
	room_centroid(
			&tox,
			&toy,
			to);

	int dx = tox - fromx;
	int dy = toy - fromy;

	int incx = dx > 0 ? 1 : -1;
	int incy = dy > 0 ? 1 : -1;

	int r = fromy, c = fromx;

	while (r!=toy && c!=tox)
	{	
		if (tmap[r][c] != ROOM)
		{
			tmap[r][c] = CORR;
			hmap[r][c] = 0;
		}

		// if we connect to existing path, then stop
		if (numAdjacentCorridors(r, c)>1)
			break;

		// prefer less hard path
		if (hmap[r][c+incx]<hmap[r+incy][c])
			c+=incx;
		else
			r+=incy;
	}
	while (r != toy || c != tox)
	{
		if (tmap[r][c] != ROOM)
		{
			tmap[r][c] = CORR;
			hmap[r][c] = 0;
		}

		// if we connect to existing path, then stop
		if (numAdjacentCorridors(r, c)>1)
			break;

		if (r!=toy)
			r+=incy;
		if (c!=tox)
			c+=incx;
	}
}

