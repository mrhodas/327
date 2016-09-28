#include "core.h"
#include "corridor.h"
#include "debug.h"
#include "dungeon.h"
#include "room.h"
#include "main.h"

char *loadp = NULL;
char *savep = NULL;

unsigned int seed = 0;

int nummon = 10;
int nummonmax = 255;



void init_dmap() {
    int r, c;

    for (r=0;r<DUNG_H;r++)
    {
        for (c=0;c<DUNG_W;c++)
        {
            dmap[r][c] = '.';
        }
    }
}

void create_distance_map(){
    init_dmap();

    p_row = rand() % DUNG_H;
    p_col = rand() % DUNG_W;

    dmap[p_row][p_col] = '@';

}


void print_distance_map()
{
    int r, c;

    for (r=0;r<DUNG_H;r++)
    {
        for (c=0;c<DUNG_W;c++)
        {
            // print dungeon cell on the dungeon
            putchar(dmap[r][c]);
        }
        putchar('\n');
    }
}



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

	/* switches */
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
				return 1;
			}
		}
	}

	if (!seed) 
		// if seed is unset
		srand(seed = time(NULL));
	else 
		// seed is set by --seed option
		srand(seed);

	debug_log("debug: seed: %d\n", seed);
	debug_log("debug: load path: %s\n", loadp);
	debug_log("debug: save path: %s\n", savep);

	if (loadp)
		dungeon_load();
	else
		dungeon_generate();

	printf("\nOriginal Dungeon:\n");
	dungeon_print();
    printf("\nDistance Map:\n");

    create_distance_map();
    print_distance_map();

    if (savep)
		dungeon_save();

	return 0;
}

