#include "core.h"
#include "corridor.h"
#include "character.h"
#include "debug.h"
#include "dungeon.h"
#include "room.h"
#include "dijkstra.h"

int pcx = 0; // 0 means unset
int pcy = 0; // 0 means unset

Character *pc;

unsigned int seed = 0;

int main(int argc, char** argv)
{
	char *home = getenv("HOME");

	// make ~/.rlg327 directory
	char dirpath[strlen(home) + strlen("/.rlg327") + 1];
	sprintf(dirpath, "%s/.rlg327", home);
	mkdir(dirpath, S_IRUSR | S_IWUSR | S_IXUSR);

	// ~/.rlg327/dungeon
	char filepath[strlen(home) + strlen("/.rlg327/dungeon") + 1];
	sprintf(filepath, "%s/.rlg327/dungeon", home);

	int i;

	/* options */
	if (argc > 1)
	{
		for (i=1; i<argc; i++)
		{
			if (!strcmp("--debug", argv[i]))
			{
				debug = 1;
			}
			else if (!strcmp("--load", argv[i]))
			{
				if (i+1<argc && argv[i+1][0]!='-')
					loadp = argv[++i];
				else
					loadp = filepath;
			}
			else if (!strcmp("--save", argv[i]))
			{
				if (i+1<argc && argv[i+1][0]!='-')
					savep = argv[++i];
				else
					savep = filepath;
			}
			else if (!strcmp("--pc", argv[i]))
			{
				if (i+2<argc)
				{
					pcx = atoi(argv[++i]);
					pcy = atoi(argv[++i]);
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --pc <x> <y>\n");
					return 1;
				}
			}
			else if (!strcmp("--seed", argv[i]))
			{
				if (i+1<argc)
				{
					seed = atoi(argv[++i]);
				}
				else
				{
					fprintf(stderr, "usage: rlg327 --seed <seed>\n");
					return 1;
				}
			}
			// undefined option
			else
			{
				fprintf(stderr, "option %s is undefined.\n", argv[i]);
				fprintf(stderr, "see available options using -h switch.\n");
				return 1;
			}
		}
	}

	if (!seed) 
		// if seed is unset
		srand(seed = time(NULL));
	else 
		// seed is set by -s option
		srand(seed);

	debug_log("debug: seed: %d\n", seed);
	debug_log("debug: save path: %s\n", savep);
	debug_log("debug: load path: %s\n", loadp);

	if (loadp)
		dungeon_load();
	else
		dungeon_generate();

	pc = character_new('@');	

	// place PC	
	if (!pcx && !pcy) // if PC location is unset
	{
		character_place(pc);
	}
	else
	{
		pc->x = pcx;
		pc->y = pcy;
		cmap[pcy][pcx] = pc;
	}
	
	// init distance map
	dijkstra_init(pc->x, pc->y, 0);
	dijkstra_init(pc->x, pc->y, 1);

	dungeon_print();

	dijkstra_print(0);
	dijkstra_print(1);
	
	if (savep)
		dungeon_save();

	character_delete(pc);
	
	return 0;
}

