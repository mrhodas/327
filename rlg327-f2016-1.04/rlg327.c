#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "dungeon.h"
#include "pc.h"
#include "npc.h"
#include "move.h"
<<<<<<< HEAD
<<<<<<< HEAD

const char *victory =
  "\n                                       o\n"
  "                                      $\"\"$o\n"
  "                                     $\"  $$\n"
  "                                      $$$$\n"
  "                                      o \"$o\n"
  "                                     o\"  \"$\n"
  "                oo\"$$$\"  oo$\"$ooo   o$    \"$    ooo\"$oo  $$$\"o\n"
  "   o o o o    oo\"  o\"      \"o    $$o$\"     o o$\"\"  o$      \"$  "
  "\"oo   o o o o\n"
  "   \"$o   \"\"$$$\"   $$         $      \"   o   \"\"    o\"         $"
  "   \"o$$\"    o$$\n"
  "     \"\"o       o  $          $\"       $$$$$       o          $  ooo"
  "     o\"\"\n"
  "        \"o   $$$$o $o       o$        $$$$$\"       $o        \" $$$$"
  "   o\"\n"
  "         \"\"o $$$$o  oo o  o$\"         $$$$$\"        \"o o o o\"  "
  "\"$$$  $\n"
  "           \"\" \"$\"     \"\"\"\"\"            \"\"$\"            \""
  "\"\"      \"\"\" \"\n"
  "            \"oooooooooooooooooooooooooooooooooooooooooooooooooooooo$\n"
  "             \"$$$$\"$$$$\" $$$$$$$\"$$$$$$ \" \"$$$$$\"$$$$$$\"  $$$\""
  "\"$$$$\n"
  "              $$$oo$$$$   $$$$$$o$$$$$$o\" $$$$$$$$$$$$$$ o$$$$o$$$\"\n"
  "              $\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""
  "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"$\n"
  "              $\"                                                 \"$\n"
  "              $\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\""
  "$\"$\"$\"$\"$\"$\"$\"$\n"
  "                                   You win!\n\n";

const char *tombstone =
  "\n\n\n\n                /\"\"\"\"\"/\"\"\"\"\"\"\".\n"
  "               /     /         \\             __\n"
  "              /     /           \\            ||\n"
  "             /____ /   Rest in   \\           ||\n"
  "            |     |    Pieces     |          ||\n"
  "            |     |               |          ||\n"
  "            |     |   A. Luser    |          ||\n"
  "            |     |               |          ||\n"
  "            |     |     * *   * * |         _||_\n"
  "            |     |     *\\/* *\\/* |        | TT |\n"
  "            |     |     *_\\_  /   ...\"\"\"\"\"\"| |"
  "| |.\"\"....\"\"\"\"\"\"\"\".\"\"\n"
  "            |     |         \\/..\"\"\"\"\"...\"\"\""
  "\\ || /.\"\"\".......\"\"\"\"...\n"
  "            |     |....\"\"\"\"\"\"\"........\"\"\"\"\""
  "\"^^^^\".......\"\"\"\"\"\"\"\"..\"\n"
  "            |......\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"......"
  "..\"\"\"\"\"....\"\"\"\"\"..\"\"...\"\"\".\n\n"
  "            You're dead.  Better luck in the next life.\n\n\n";

void usage(char *name)
{
  fprintf(stderr,
          "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
          "       [-i|--image <pgm>] [-s|--save] [-p|--pc <y> <x>]\n"
          "       [-n|--nummon <monster count>\n",
          name);

  exit(-1);
}

int main(int argc, char *argv[])
{
  dungeon_t d;
  time_t seed;
  struct timeval tv;
  uint32_t i;
  uint32_t do_load, do_save, do_seed, do_image, do_place_pc;
  uint32_t long_arg;
  char *save_file;
  char *pgm_file;

  memset(&d, 0, sizeof (d));

  /* Default behavior: Seed with the time, generate a new dungeon, *
   * and don't write to disk.                                      */
  do_load = do_save = do_image = do_place_pc = 0;
  do_seed = 1;
  save_file = NULL;
  d.max_monsters = 10;

  /* The project spec requires '--load' and '--save'.  It's common  *
   * to have short and long forms of most switches (assuming you    *
   * don't run out of letters).  For now, we've got plenty.  Long   *
   * forms use whole words and take two dashes.  Short forms use an *
`   * abbreviation after a single dash.  We'll add '--rand' (to     *
   * specify a random seed), which will take an argument of it's    *
   * own, and we'll add short forms for all three commands, '-l',   *
   * '-s', and '-r', respectively.  We're also going to allow an    *
   * optional argument to load to allow us to load non-default save *
   * files.  No means to save to non-default locations, however.    *
   * And the final switch, '--image', allows me to create a dungeon *
   * from a PGM image, so that I was able to create those more      *
   * interesting test dungeons for you.                             */
 
// if (argc > 1) {
//    for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0) {
//      if (argv[i][0] == '-') { /* All switches start with a dash */
//        if (argv[i][1] == '-') {
//          argv[i]++;    /* Make the argument have a single dash so we can */
//          long_arg = 1; /* handle long and short args at the same place.  */
//        }
//        switch (argv[i][1]) {
//        case 'r':
//          if ((!long_arg && argv[i][2]) ||
//              (long_arg && strcmp(argv[i], "-rand")) ||
//              argc < ++i + 1 /* No more arguments */ ||
//              !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */) {
//            usage(argv[0]);
//          }
//          do_seed = 0;
//          break;
//        case 'l':
//          if ((!long_arg && argv[i][2]) ||
//              (long_arg && strcmp(argv[i], "-load"))) {
//            usage(argv[0]);
//          }
//          do_load = 1;
//          if ((argc > i + 1) && argv[i + 1][0] != '-') {
//            /* There is another argument, and it's not a switch, so *
//             * we'll treat it as a save file and try to load it.    */
//            save_file = argv[++i];
//          }
//          break;
//        case 's':
//          if ((!long_arg && argv[i][2]) ||
//              (long_arg && strcmp(argv[i], "-save"))) {
//            usage(argv[0]);
//          }
//          do_save = 1;
//          break;
//        case 'i':
//          if ((!long_arg && argv[i][2]) ||
//              (long_arg && strcmp(argv[i], "-image"))) {
//            usage(argv[0]);
//          }
//          do_image = 1;
//          if ((argc > i + 1) && argv[i + 1][0] != '-') {
//            /* There is another argument, and it's not a switch, so *
//             * we'll treat it as a save file and try to load it.    */
//            pgm_file = argv[++i];
//          }
//          break;
//        case 'n':
//          if ((!long_arg && argv[i][2]) ||
//              (long_arg && strcmp(argv[i], "-nummon")) ||
//              argc < ++i + 1 /* No more arguments */ ||
//              !sscanf(argv[i], "%hu", &d.max_monsters)) {
//            usage(argv[0]);
//          }
//          break;
//        case 'p':
//          if ((!long_arg && argv[i][2])            ||
//              (long_arg && strcmp(argv[i], "-pc")) ||
//              argc <= i + 2                        ||
//              argv[i + 1][0] == '-'                ||
//              argv[i + 2][0] == '-') {
//            usage(argv[0]);
//          }
//          do_place_pc = 1;
//          if ((d.pc.position[dim_y] = atoi(argv[++i])) < 1 ||
//              d.pc.position[dim_y] > DUNGEON_Y - 2         ||
//              (d.pc.position[dim_x] = atoi(argv[++i])) < 1 ||
//              d.pc.position[dim_x] > DUNGEON_X - 2) {
//            fprintf(stderr, "Invalid PC position.\n");
//            usage(argv[0]);
//          }
//          break;
//        default:
//          usage(argv[0]);
//        }
//      } else { /* No dash */
        //usage(argv[0]);
//      }
//    }


  if (do_seed) {
    /* Allows me to generate more than one dungeon *
     * per second, as opposed to time().           */
    gettimeofday(&tv, NULL);
    seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
  }

  printf("Seed is %ld.\n", seed);
  srand(seed);

  init_dungeon(&d);

//  if (do_load) {
//    read_dungeon(&d, save_file);
//  } else if (do_image) {
//    read_pgm(&d, pgm_file);
//  } else {
    gen_dungeon(&d);
//  }

  config_pc(&d);
  gen_monsters(&d);

  while (pc_is_alive(&d) && dungeon_has_npcs(&d)) {
    render_dungeon(&d);
    do_moves(&d);
    usleep(250000);
  }
  render_dungeon(&d);

//  if (do_save) {
//    write_dungeon(&d);
//  }

  printf(pc_is_alive(&d) ? victory : tombstone);

  pc_delete(d.pc.pc);
  delete_dungeon(&d);

  return 0;
=======
//#include <ncurses.h>
=======

#define NUMBER_OF_VALID_CHARS 22
>>>>>>> d5990dd89eb49a9628466cae98a9f0995c09dac2

const char *victory =
        "\n                                       o\n"
                "                                      $\"\"$o\n"
                "                                     $\"  $$\n"
                "                                      $$$$\n"
                "                                      o \"$o\n"
                "                                     o\"  \"$\n"
                "                oo\"$$$\"  oo$\"$ooo   o$    \"$    ooo\"$oo  $$$\"o\n"
                "   o o o o    oo\"  o\"      \"o    $$o$\"     o o$\"\"  o$      \"$  "
                "\"oo   o o o o\n"
                "   \"$o   \"\"$$$\"   $$         $      \"   o   \"\"    o\"         $"
                "   \"o$$\"    o$$\n"
                "     \"\"o       o  $          $\"       $$$$$       o          $  ooo"
                "     o\"\"\n"
                "        \"o   $$$$o $o       o$        $$$$$\"       $o        \" $$$$"
                "   o\"\n"
                "         \"\"o $$$$o  oo o  o$\"         $$$$$\"        \"o o o o\"  "
                "\"$$$  $\n"
                "           \"\" \"$\"     \"\"\"\"\"            \"\"$\"            \""
                "\"\"      \"\"\" \"\n"
                "            \"oooooooooooooooooooooooooooooooooooooooooooooooooooooo$\n"
                "             \"$$$$\"$$$$\" $$$$$$$\"$$$$$$ \" \"$$$$$\"$$$$$$\"  $$$\""
                "\"$$$$\n"
                "              $$$oo$$$$   $$$$$$o$$$$$$o\" $$$$$$$$$$$$$$ o$$$$o$$$\"\n"
                "              $\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\""
                "\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"$\n"
                "              $\"                                                 \"$\n"
                "              $\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\"$\""
                "$\"$\"$\"$\"$\"$\"$\"$\n"
                "                                   You win!\n\n";

const char *tombstone =
        "\n\n\n\n                /\"\"\"\"\"/\"\"\"\"\"\"\".\n"
                "               /     /         \\             __\n"
                "              /     /           \\            ||\n"
                "             /____ /   Rest in   \\           ||\n"
                "            |     |    Pieces     |          ||\n"
                "            |     |               |          ||\n"
                "            |     |   A. Luser    |          ||\n"
                "            |     |               |          ||\n"
                "            |     |     * *   * * |         _||_\n"
                "            |     |     *\\/* *\\/* |        | TT |\n"
                "            |     |     *_\\_  /   ...\"\"\"\"\"\"| |"
                "| |.\"\"....\"\"\"\"\"\"\"\".\"\"\n"
                "            |     |         \\/..\"\"\"\"\"...\"\"\""
                "\\ || /.\"\"\".......\"\"\"\"...\n"
                "            |     |....\"\"\"\"\"\"\"........\"\"\"\"\""
                "\"^^^^\".......\"\"\"\"\"\"\"\"..\"\n"
                "            |......\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"......"
                "..\"\"\"\"\"....\"\"\"\"\"..\"\"...\"\"\".\n\n"
                "            You're dead.  Better luck in the next life.\n\n\n";

void usage(char *name) {
    fprintf(stderr,
            "Usage: %s [-r|--rand <seed>] [-l|--load [<file>]]\n"
                    "       [-i|--image <pgm>] [-s|--save] [-p|--pc <y> <x>]\n"
                    "       [-n|--nummon <monster count>\n",
            name);

    exit(-1);
}

int valid_input(char ch);

int main(int argc, char *argv[]) {
    dungeon_t d;
    time_t seed;
    struct timeval tv;
    uint32_t i;
    uint32_t do_load, do_save, do_seed, do_image, do_place_pc;
    uint32_t long_arg;
    char *save_file;
    char *pgm_file;

    memset(&d, 0, sizeof(d));

    /* Default behavior: Seed with the time, generate a new dungeon, *
     * and don't write to disk.                                      */
    do_load = do_save = do_image = do_place_pc = 0;
    do_seed = 1;
    save_file = NULL;
    d.max_monsters = 10;

    /* The project spec requires '--load' and '--save'.  It's common  *
     * to have short and long forms of most switches (assuming you    *
     * don't run out of letters).  For now, we've got plenty.  Long   *
     * forms use whole words and take two dashes.  Short forms use an *
  `   * abbreviation after a single dash.  We'll add '--rand' (to     *
     * specify a random seed), which will take an argument of it's    *
     * own, and we'll add short forms for all three commands, '-l',   *
     * '-s', and '-r', respectively.  We're also going to allow an    *
     * optional argument to load to allow us to load non-default save *
     * files.  No means to save to non-default locations, however.    *
     * And the final switch, '--image', allows me to create a dungeon *
     * from a PGM image, so that I was able to create those more      *
     * interesting test dungeons for you.                             */

    if (argc > 1) {
        for (i = 1, long_arg = 0; i < argc; i++, long_arg = 0) {
            if (argv[i][0] == '-') { /* All switches start with a dash */
                if (argv[i][1] == '-') {
                    argv[i]++;    /* Make the argument have a single dash so we can */
                    long_arg = 1; /* handle long and short args at the same place.  */
                }
                switch (argv[i][1]) {
                    case 'r':
                        if ((!long_arg && argv[i][2]) ||
                            (long_arg && strcmp(argv[i], "-rand")) ||
                            argc < ++i + 1 /* No more arguments */ ||
                            !sscanf(argv[i], "%lu", &seed) /* Argument is not an integer */) {
                            usage(argv[0]);
                        }
                        do_seed = 0;
                        break;
                    case 'l':
                        if ((!long_arg && argv[i][2]) ||
                            (long_arg && strcmp(argv[i], "-load"))) {
                            usage(argv[0]);
                        }
                        do_load = 1;
                        if ((argc > i + 1) && argv[i + 1][0] != '-') {
                            /* There is another argument, and it's not a switch, so *
                             * we'll treat it as a save file and try to load it.    */
                            save_file = argv[++i];
                        }
                        break;
                    case 's':
                        if ((!long_arg && argv[i][2]) ||
                            (long_arg && strcmp(argv[i], "-save"))) {
                            usage(argv[0]);
                        }
                        do_save = 1;
                        break;
                    case 'i':
                        if ((!long_arg && argv[i][2]) ||
                            (long_arg && strcmp(argv[i], "-image"))) {
                            usage(argv[0]);
                        }
                        do_image = 1;
                        if ((argc > i + 1) && argv[i + 1][0] != '-') {
                            /* There is another argument, and it's not a switch, so *
                             * we'll treat it as a save file and try to load it.    */
                            pgm_file = argv[++i];
                        }
                        break;
                    case 'n':
                        if ((!long_arg && argv[i][2]) ||
                            (long_arg && strcmp(argv[i], "-nummon")) ||
                            argc < ++i + 1 /* No more arguments */ ||
                            !sscanf(argv[i], "%hu", &d.max_monsters)) {
                            usage(argv[0]);
                        }
                        break;
                    case 'p':
                        if ((!long_arg && argv[i][2]) ||
                            (long_arg && strcmp(argv[i], "-pc")) ||
                            argc <= i + 2 ||
                            argv[i + 1][0] == '-' ||
                            argv[i + 2][0] == '-') {
                            usage(argv[0]);
                        }
                        do_place_pc = 1;
                        if ((d.pc.position[dim_y] = atoi(argv[++i])) < 1 ||
                            d.pc.position[dim_y] > DUNGEON_Y - 2 ||
                            (d.pc.position[dim_x] = atoi(argv[++i])) < 1 ||
                            d.pc.position[dim_x] > DUNGEON_X - 2) {
                            fprintf(stderr, "Invalid PC position.\n");
                            usage(argv[0]);
                        }
                        break;
                    default:
                        usage(argv[0]);
                }
            } else { /* No dash */
                usage(argv[0]);
            }
        }
    }

    if (do_seed) {
        /* Allows me to generate more than one dungeon *
         * per second, as opposed to time().           */
        gettimeofday(&tv, NULL);
        seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
    }

    //printf("Seed is %ld.\n", seed);
    srand(seed);
    init_dungeon(&d);

    if (do_load) {
        read_dungeon(&d, save_file);
    } else if (do_image) {
        read_pgm(&d, pgm_file);
    } else {
        gen_dungeon(&d);
    }
    config_pc(&d);
    gen_monsters(&d);


    //init ncurses.
    initscr();
    noecho();
    curs_set(FALSE);
    cbreak();
    keypad(stdscr, TRUE);

    // init windows
    msg_win = newwin(1, DUNGEON_X, 0, 0);
    dun_win = newwin(DUNGEON_Y, DUNGEON_X, 1, 0);
    btm_win = newwin(2, DUNGEON_X, DUNGEON_Y + 2, 0);
    mon_win = newwin(10, 20, 10, 30);

    //Fill bottom with -'s
    wmove(btm_win, 0, 0);
    for (i = 1; i < (DUNGEON_X * 2) + 1; i++) {
        waddch(btm_win, '-');
    }

    render_dungeon(&d);
    wrefresh(dun_win);
    wrefresh(btm_win);
    wrefresh(msg_win);
    refresh();

    int moves = 1;
    char input;
    i = 0;
    while (pc_is_alive(&d) && dungeon_has_npcs(&d)) {

        if ((input = getch()) == ERR) {
            mvwprintw(msg_win, 0, 0, "There was an ERR in input.");
            wrefresh(msg_win);
        } else {
            if (valid_input(input)) {
                if (input == 'Q') {
                    endwin();
                    delete_dungeon(&d);
                    pc_delete(d.pc.pc);
                    printf("\nProgram Ended...\n\n");
                    return 0;
                }
                if (input == 'm') {
                    moves = 0;
                    int y;
                    for (y = 0; y < d.num_monsters; y++) {
                        mvwprintw(mon_win, y, 0, "%d. Monster %d", y + 1, y + 1);
                        if (y == 9) {
                            break;
                        }
                    }
                    wrefresh(mon_win);
                    int flag = 1;
                    int offset = 0;
                    while (flag) {
                        char in = getch();
                        if (in == KEY_UP) {
                            if (offset > 0) {
                                offset--;
                                wclear(mon_win);
                                for (y = offset; y < d.num_monsters; y++) {
                                    mvwprintw(mon_win, y, 0, "%d. Monster %d", y + 1 + offset, y + 1 + offset);
                                    if (y == 9) {
                                        break;
                                    }
                                }
                                wrefresh(mon_win);
                            }
                        }
                        if (in == KEY_DOWN) {
                            if (offset < d.num_monsters) {
                                offset++;
                                wclear(mon_win);
                                for (y = offset; y < d.num_monsters; y++) {
                                    mvwprintw(mon_win, y, 0, "%d. Monster %d", y + 1 + offset, y + 1 + offset);
                                    if (y == 9) {
                                        break;
                                    }
                                }
                                wrefresh(mon_win);
                            }
                        }

                        if (in == 27) {
                            flag = 0;
                            wclear(mon_win);
                            wrefresh(mon_win);
                            wrefresh(btm_win);
                        }
                    }
                }
                if (input == '>') {
                    moves = 0;
                    if (d.map[d.pc.position[dim_y]][d.pc.position[dim_x]]
                        == ter_stair_up) {
                        //delete_dungeon(&d);
                        //character_delete(d.pc.pc);
                        init_dungeon(&d);
                        gen_dungeon(&d);
                        config_pc(&d);
                        gen_monsters(&d);
                        render_dungeon(&d);
                        continue;
                    }
                }
                if (input == '<') {
                    moves = 0;
                    if (d.map[d.pc.position[dim_y]][d.pc.position[dim_x]]
                        == ter_stair_down) {
                        //delete_dungeon(&d);
                        //character_delete(d.pc.pc);
                        init_dungeon(&d);
                        gen_dungeon(&d);
                        config_pc(&d);
                        gen_monsters(&d);
                        render_dungeon(&d);
                        continue;
                    }
                }
                mvwprintw(msg_win, 0, 0, "%c was pressed.", input);
                wrefresh(msg_win);
                if (moves) {
                    do_moves(&d, input);
                }
                moves = 1;
            }
        }
        render_dungeon(&d);
        wrefresh(dun_win);
        wrefresh(btm_win);
        refresh();
    }

    endwin();

    if (do_save) {
        write_dungeon(&d);
    }
    printf(pc_is_alive(&d) ? victory : tombstone);

    pc_delete(d.pc.pc);
    delete_dungeon(&d);

    return 0;
>>>>>>> afaa278bbb54864f37bfd875d2473e3eeeddab5d
}

int valid_input(char ch) {
    char validChars[NUMBER_OF_VALID_CHARS] = "123456789ykulnjbh<> mQ";
    int i;

    for (i = 0; i < NUMBER_OF_VALID_CHARS; i++) {
        if (ch == validChars[i]) {
            return 1;
        }
    }

    if (ch == KEY_UP || ch == KEY_DOWN) {
        return 1;
    }

    return 0;
}
