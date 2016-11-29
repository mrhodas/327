#include <unistd.h>
#include <ncurses.h>
#include <ctype.h>
#include <stdlib.h>
#include <sstream>

#include "io.h"
#include "move.h"
#include "path.h"
#include "pc.h"
#include "utils.h"
#include "dungeon.h"
#include "macros.h"
#include "object.h"

/* Same ugly hack we did in path.c */
static dungeon_t *dungeon;

typedef struct io_message {
    /* Will print " --more-- " at end of line when another message follows. *
     * Leave 10 extra spaces for that.                                      */
    char msg[71];
    struct io_message *next;
} io_message_t;

static io_message_t *io_head, *io_tail;

void io_init_terminal(void) {
    initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
}

void io_reset_terminal(void) {
    endwin();

    while (io_head) {
        io_tail = io_head;
        io_head = io_head->next;
        free(io_tail);
    }
    io_tail = NULL;
}

void io_queue_message(const char *format, ...) {
    io_message_t *tmp;
    va_list ap;

    if (!(tmp = (io_message_t *) malloc(sizeof(*tmp)))) {
        perror("malloc");
        exit(1);
    }

    tmp->next = NULL;

    va_start(ap, format);

    vsnprintf(tmp->msg, sizeof(tmp->msg), format, ap);

    va_end(ap);

    if (!io_head) {
        io_head = io_tail = tmp;
    } else {
        io_tail->next = tmp;
        io_tail = tmp;
    }
}

static void io_print_message_queue(uint32_t y, uint32_t x) {
    while (io_head) {
        io_tail = io_head;
        attron(COLOR_PAIR(COLOR_CYAN));
        mvprintw(y, x, "%-80s", io_head->msg);
        attroff(COLOR_PAIR(COLOR_CYAN));
        io_head = io_head->next;
        if (io_head) {
            attron(COLOR_PAIR(COLOR_CYAN));
            mvprintw(y, x + 70, "%10s", " --more-- ");
            attroff(COLOR_PAIR(COLOR_CYAN));
            refresh();
            getch();
        }
        free(io_tail);
    }
    io_tail = NULL;
}

static char distance_to_char[] =
        "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void io_display_tunnel(dungeon_t *d) {
    uint32_t y, x;
    clear();
    for (y = 0; y < DUNGEON_Y; y++) {
        for (x = 0; x < DUNGEON_X; x++) {
            mvaddch(y + 1, x, (d->pc_tunnel[y][x] < 62 ?
                               distance_to_char[d->pc_tunnel[y][x]] :
                               '*'));
        }
    }
    refresh();
}

void io_display_distance(dungeon_t *d) {
    uint32_t y, x;
    clear();
    for (y = 0; y < DUNGEON_Y; y++) {
        for (x = 0; x < DUNGEON_X; x++) {
            mvaddch(y + 1, x, (d->pc_distance[y][x] < 62 ?
                               distance_to_char[d->pc_distance[y][x]] :
                               '*'));
        }
    }
    refresh();
}

void io_display_hardness(dungeon_t *d) {
    uint32_t y, x;
    clear();
    for (y = 0; y < DUNGEON_Y; y++) {
        for (x = 0; x < DUNGEON_X; x++) {
            mvaddch(y + 1, x,
                    (d->hardness[y][x] ?
                     distance_to_char[1 +
                                      ((int) (((float) d->hardness[y][x]) *
                                              (60.0 / 255.0)))] :
                     '0'));
        }
    }
    refresh();
}

void io_display_all(dungeon_t *d) {
    uint32_t y, x;

    clear();
    for (y = 0; y < DUNGEON_Y; y++) {
        for (x = 0; x < DUNGEON_X; x++) {
            if (d->charmap[y][x]) {
                attron(COLOR_PAIR(d->charmap[y][x]->get_color()));
                mvaddch(y + 1, x, d->charmap[y][x]->get_symbol());
                attroff(COLOR_PAIR(d->charmap[y][x]->get_color()));
            } else if (d->objmap[y][x]) {
                attron(COLOR_PAIR(d->objmap[y][x]->get_color()));
                mvaddch(y + 1, x, d->objmap[y][x]->get_symbol());
                attroff(COLOR_PAIR(d->objmap[y][x]->get_color()));
            } else {
                switch (mapxy(x, y)) {
                    case ter_wall:
                    case ter_wall_immutable:
                        mvaddch(y + 1, x, ' ');
                        break;
                    case ter_floor:
                    case ter_floor_room:
                        mvaddch(y + 1, x, '.');
                        break;
                    case ter_floor_hall:
                        mvaddch(y + 1, x, '#');
                        break;
                    case ter_debug:
                        mvaddch(y + 1, x, '*');
                        break;
                    case ter_stairs_up:
                        mvaddch(y + 1, x, '<');
                        break;
                    case ter_stairs_down:
                        mvaddch(y + 1, x, '>');
                        break;
                    default:
                        /* Use zero as an error symbol, since it stands out somewhat, and it's *
                         * not otherwise used.                                                 */
                        mvaddch(y + 1, x, '0');
                }
            }
        }
    }

    io_print_message_queue(0, 0);

    refresh();
}

void io_display(dungeon_t *d) {
    pair_t pos;
    uint32_t illuminated;

    clear();
    for (pos[dim_y] = 0; pos[dim_y] < DUNGEON_Y; pos[dim_y]++) {
        for (pos[dim_x] = 0; pos[dim_x] < DUNGEON_X; pos[dim_x]++) {
            if ((illuminated = is_illuminated(d->thepc, pos[dim_y], pos[dim_x]))) {
                attron(A_BOLD);
            }
            if (d->charmap[pos[dim_y]][pos[dim_x]] &&
                can_see(d, character_get_pos(d->thepc), pos, 1)) {
                attron(COLOR_PAIR(d->charmap[pos[dim_y]][pos[dim_x]]->get_color()));
                mvaddch(pos[dim_y] + 1, pos[dim_x],
                        d->charmap[pos[dim_y]][pos[dim_x]]->get_symbol());
                attroff(COLOR_PAIR(d->charmap[pos[dim_y]][pos[dim_x]]->get_color()));
            } else if (d->objmap[pos[dim_y]][pos[dim_x]] &&
                       (d->objmap[pos[dim_y]][pos[dim_x]]->have_seen() ||
                        can_see(d, character_get_pos(d->thepc), pos, 1))) {
                attron(COLOR_PAIR(d->objmap[pos[dim_y]][pos[dim_x]]->get_color()));
                mvaddch(pos[dim_y] + 1, pos[dim_x],
                        d->objmap[pos[dim_y]][pos[dim_x]]->get_symbol());
                attroff(COLOR_PAIR(d->objmap[pos[dim_y]][pos[dim_x]]->get_color()));
            } else {
                switch (pc_learned_terrain(d->thepc, pos[dim_y], pos[dim_x])) {
                    case ter_wall:
                    case ter_wall_immutable:
                    case ter_unknown:
                        mvaddch(pos[dim_y] + 1, pos[dim_x], ' ');
                        break;
                    case ter_floor:
                    case ter_floor_room:
                        mvaddch(pos[dim_y] + 1, pos[dim_x], '.');
                        break;
                    case ter_floor_hall:
                        mvaddch(pos[dim_y] + 1, pos[dim_x], '#');
                        break;
                    case ter_debug:
                        mvaddch(pos[dim_y] + 1, pos[dim_x], '*');
                        break;
                    case ter_stairs_up:
                        mvaddch(pos[dim_y] + 1, pos[dim_x], '<');
                        break;
                    case ter_stairs_down:
                        mvaddch(pos[dim_y] + 1, pos[dim_x], '>');
                        break;
                    default:
                        /* Use zero as an error symbol, since it stands out somewhat, and it's *
                         * not otherwise used.                                                 */
                        mvaddch(pos[dim_y] + 1, pos[dim_x], '0');
                }
            }
            if (illuminated) {
                attroff(A_BOLD);
            }
        }
    }

    mvprintw(DUNGEON_Y + 1, 0, "Inventory size: %u Equipment size: %u", d->thepc->inventory.size(),
             d->thepc->equipment.size());
    mvprintw(DUNGEON_Y + 2, 0, "Player HP: %u", d->thepc->hp);

    mvprintw(DUNGEON_Y + 2, 25, "speed: %u", d->thepc->speed);
    mvprintw(DUNGEON_Y + 2, 40, "dmg: %u", d->thepc->damage_bonus());

    io_print_message_queue(0, 0);

    refresh();
}

void io_display_monster_list(dungeon_t *d) {
    mvprintw(11, 33, " HP:    XXXXX ");
    mvprintw(12, 33, " Speed: XXXXX ");
    mvprintw(14, 27, " Hit any key to continue. ");
    refresh();
    getch();
}

uint32_t io_teleport_pc(dungeon_t *d) {
    /* Just for fun. */
    pair_t dest;

    do {
        dest[dim_x] = rand_range(1, DUNGEON_X - 2);
        dest[dim_y] = rand_range(1, DUNGEON_Y - 2);
    } while (charpair(dest));

    d->charmap[character_get_y(d->thepc)][character_get_x(d->thepc)] = NULL;
    d->charmap[dest[dim_y]][dest[dim_x]] = d->thepc;

    character_set_y(d->thepc, dest[dim_y]);
    character_set_x(d->thepc, dest[dim_x]);

    if (mappair(dest) < ter_floor) {
        mappair(dest) = ter_floor;
    }

    pc_observe_terrain(d->thepc, d);

    dijkstra(d);
    dijkstra_tunnel(d);

    return 0;
}

/* Adjectives to describe our monsters */
static const char *adjectives[] = {
        "A menacing ",
        "A threatening ",
        "A horrifying ",
        "An intimidating ",
        "An aggressive ",
        "A frightening ",
        "A terrifying ",
        "A terrorizing ",
        "An alarming ",
        "A frightening ",
        "A dangerous ",
        "A glowering ",
        "A glaring ",
        "A scowling ",
        "A chilling ",
        "A scary ",
        "A creepy ",
        "An eerie ",
        "A spooky ",
        "A slobbering ",
        "A drooling ",
        " A horrendous ",
        "An unnerving ",
        "A cute little ",  /* Even though they're trying to kill you, */
        "A teeny-weenie ", /* they can still be cute!                 */
        "A fuzzy ",
        "A fluffy white ",
        "A kawaii ",       /* For our otaku */
        "Hao ke ai de "    /* And for our Chinese */
        /* And there's one special case (see below) */
};

static void io_scroll_monster_list(char (*s)[40], uint32_t count) {
    uint32_t offset;
    uint32_t i;

    offset = 0;

    while (1) {
        for (i = 0; i < 13; i++) {
            mvprintw(i + 6, 19, " %-40s ", s[i + offset]);
        }
        switch (getch()) {
            case KEY_UP:
                if (offset) {
                    offset--;
                }
                break;
            case KEY_DOWN:
                if (offset < (count - 13)) {
                    offset++;
                }
                break;
            case 27:
                return;
        }

    }
}

static bool is_vowel(const char c) {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

static void io_list_monsters_display(dungeon_t *d,
                                     character **c,
                                     uint32_t count) {
    uint32_t i;
    char (*s)[40]; /* pointer to array of 40 char */

    (void) adjectives;

    s = (char (*)[40]) malloc(count * sizeof(*s));

    mvprintw(3, 19, " %-40s ", "");
    /* Borrow the first element of our array for this string: */
    snprintf(s[0], 40, "You know of %d monsters:", count);
    mvprintw(4, 19, " %-40s ", s);
    mvprintw(5, 19, " %-40s ", "");

    for (i = 0; i < count; i++) {
        snprintf(s[i], 40, "%3s%s (%c): %2d %s by %2d %s",
                 (is_vowel(character_get_name(c[i])[0]) ? "An " : "A "),
                 character_get_name(c[i]),
                 character_get_symbol(c[i]),
                 abs(character_get_y(c[i]) - character_get_y(d->thepc)),
                 ((character_get_y(c[i]) - character_get_y(d->thepc)) <= 0 ?
                  "North" : "South"),
                 abs(character_get_x(c[i]) - character_get_x(d->thepc)),
                 ((character_get_x(c[i]) - character_get_x(d->thepc)) <= 0 ?
                  "East" : "West"));
        if (count <= 13) {
            /* Handle the non-scrolling case right here. *
             * Scrolling in another function.            */
            mvprintw(i + 6, 19, " %-40s ", s[i]);
        }
    }

    if (count <= 13) {
        mvprintw(count + 6, 19, " %-40s ", "");
        mvprintw(count + 7, 19, " %-40s ", "Hit escape to continue.");
        while (getch() != 27 /* escape */);
    } else {
        mvprintw(19, 19, " %-40s ", "");
        mvprintw(20, 19, " %-40s ",
                 "Arrows to scroll, escape to continue.");
        io_scroll_monster_list(s, count);
    }

    free(s);
}

static int compare_monster_distance(const void *v1, const void *v2) {
    const character *const *c1 = (const character **) v1;
    const character *const *c2 = (const character **) v2;

    return (dungeon->pc_distance[character_get_y(*c1)][character_get_x(*c2)] -
            dungeon->pc_distance[character_get_y(*c1)][character_get_x(*c2)]);
}

static void io_list_monsters(dungeon_t *d) {
    character **c;
    int32_t x, y, count;
    pair_t from, to;

    from[dim_y] = max(1, character_get_y(d->thepc) - PC_VISUAL_RANGE);
    from[dim_x] = max(1, character_get_x(d->thepc) - PC_VISUAL_RANGE);
    to[dim_y] = min(DUNGEON_Y - 1, character_get_y(d->thepc) + PC_VISUAL_RANGE);
    to[dim_x] = min(DUNGEON_X - 1, character_get_x(d->thepc) + PC_VISUAL_RANGE);

    c = (character **) malloc(d->num_monsters * sizeof(*c));

    /* Get a linear list of monsters */
    for (count = 0, y = from[dim_y]; y <= to[dim_y]; y++) {
        for (x = from[dim_x]; x <= to[dim_x]; x++) {
            if (d->charmap[y][x] && d->charmap[y][x] != d->thepc) {
                c[count++] = d->charmap[y][x];
            }
        }
    }

    /* Sort it by distance from PC */
    dungeon = d;
    qsort(c, count, sizeof(*c), compare_monster_distance);

    /* Display it */
    io_list_monsters_display(d, c, count);
    free(c);

    /* And redraw the dungeon */
    io_display(d);
}

static void io_list_inventory(dungeon_t *d, char in) {
    uint i;
    clear();
    mvprintw(3, 20, "INVENTORY LIST:");
    for (i = 0; i < 10; i++) {
        std::ostringstream line;
        line << i << ":                  ";
        mvprintw(4 + i, 20, line.str().c_str());
    }

    if (d->thepc->inventory.size() > 0) {
        for (i = 0; i < d->thepc->inventory.size(); i++) {
            if (d->thepc->inventory[i]) {
                mvprintw(4 + i, 23, d->thepc->inventory[i]->get_name());
            }
        }
    }

    if (in == 'w') {
        mvprintw(2, 20, "SELECT AN ITEM TO EQUIP");
        equip_item(d);
    } else if (in == 'x') {
        mvprintw(2, 20, "SELECT AN ITEM TO EXPUNGE");
        expunge_item(d);
    } else if (in == 'd') {
        mvprintw(2, 20, "SELECT AN ITEM TO DROP");
        drop_item(d);
    } else if (in == 'I') {
        mvprintw(2, 20, "SELECT AN ITEM TO INSPECT");
        inspect_item(d);
    } else {
        esc_pause();
    }
    d->thepc->update_speed_bonus();
    io_display(d);
}

void esc_pause() {
    while (1) {
        switch (getch()) {
            case 27:
                return;
        }
    }
}

void equip_item(dungeon_t *d) {
    while (1) {
        switch (getch()) {
            case 27:
                return;
            case '0':
                if (d->thepc->equip(0)) {
                    return;
                }
                break;
            case '1':
                if (d->thepc->equip(1)) {
                    return;
                }
                break;
            case '2':
                if (d->thepc->equip(2)) {
                    return;
                }
                break;
            case '3':
                if (d->thepc->equip(3)) {
                    return;
                }
                break;
            case '4':
                if (d->thepc->equip(4)) {
                    return;
                }
                break;
            case '5':
                if (d->thepc->equip(5)) {
                    return;
                }
                break;
            case '6':
                if (d->thepc->equip(6)) {
                    return;
                }
                break;
            case '7':
                if (d->thepc->equip(7)) {
                    return;
                }
                break;
            case '8':
                if (d->thepc->equip(8)) {
                    return;
                }
                break;
            case '9':
                if (d->thepc->equip(9)) {
                    return;
                }
                break;
            default:
                break;
        }

    }
}

void expunge_item(dungeon_t *d) {
    while (1) {
        switch (getch()) {
            case 27:
                return;
            case '0':
                if (d->thepc->expunge(0)) {
                    return;
                }
                break;
            case '1':
                if (d->thepc->expunge(1)) {
                    return;
                }
                break;
            case '2':
                if (d->thepc->expunge(2)) {
                    return;
                }
                break;
            case '3':
                if (d->thepc->expunge(3)) {
                    return;
                }
                break;
            case '4':
                if (d->thepc->expunge(4)) {
                    return;
                }
                break;
            case '5':
                if (d->thepc->expunge(5)) {
                    return;
                }
                break;
            case '6':
                if (d->thepc->expunge(6)) {
                    return;
                }
                break;
            case '7':
                if (d->thepc->expunge(7)) {
                    return;
                }
                break;
            case '8':
                if (d->thepc->expunge(8)) {
                    return;
                }
                break;
            case '9':
                if (d->thepc->expunge(9)) {
                    return;
                }
                break;
            default:
                break;
        }

    }
}

void drop_item(dungeon_t *d) {
    while (1) {
        switch (getch()) {
            case 27:
                return;
            case '0':
                if (d->thepc->drop(0, d)) {
                    return;
                }
                break;
            case '1':
                if (d->thepc->drop(1, d)) {
                    return;
                }
                break;
            case '2':
                if (d->thepc->drop(2, d)) {
                    return;
                }
                break;
            case '3':
                if (d->thepc->drop(3, d)) {
                    return;
                }
                break;
            case '4':
                if (d->thepc->drop(4, d)) {
                    return;
                }
                break;
            case '5':
                if (d->thepc->drop(5, d)) {
                    return;
                }
                break;
            case '6':
                if (d->thepc->drop(6, d)) {
                    return;
                }
                break;
            case '7':
                if (d->thepc->drop(7, d)) {
                    return;
                }
                break;
            case '8':
                if (d->thepc->drop(8, d)) {
                    return;
                }
                break;
            case '9':
                if (d->thepc->drop(9, d)) {
                    return;
                }
                break;
            default:
                break;
        }

    }
}

void inspect_item(dungeon_t *d) {
    while (1) {
        switch (getch()) {
            case 27:
                return;
            case '0':
                if (d->thepc->inspect(0)) {
                    return;
                }
                break;
            case '1':
                if (d->thepc->inspect(1)) {
                    return;
                }
                break;
            case '2':
                if (d->thepc->inspect(2)) {
                    return;
                }
                break;
            case '3':
                if (d->thepc->inspect(3)) {
                    return;
                }
                break;
            case '4':
                if (d->thepc->inspect(4)) {
                    return;
                }
                break;
            case '5':
                if (d->thepc->inspect(5)) {
                    return;
                }
                break;
            case '6':
                if (d->thepc->inspect(6)) {
                    return;
                }
                break;
            case '7':
                if (d->thepc->inspect(7)) {
                    return;
                }
                break;
            case '8':
                if (d->thepc->inspect(8)) {
                    return;
                }
                break;
            case '9':
                if (d->thepc->inspect(9)) {
                    return;
                }
                break;
            default:
                break;
        }

    }
}

void display_object_desc(std::string str) {
    clear();
    mvprintw(1, 0, "DESCRIPTION:       (ESC to continue)");
    mvprintw(2, 0, "%s", str.c_str());
    refresh();
    esc_pause();
}

static void io_list_equipment(dungeon_t *d, char in) {
    uint i;

    char equ[] = "abcdefghijkl";

    std::string label[] = {"WEAPON",
                           "OFFHAND",
                           "RANGED",
                           "ARMOR",
                           "HELMET",
                           "CLOAK",
                           "GLOVES",
                           "BOOTS",
                           "AMULET",
                           "LIGHT",
                           "RING 1",
                           "RING 2"};


    clear();
    mvprintw(3, 20, "EQUIPMENT LIST:");
    for (i = 0; i < 12; i++) {
        std::string space = "";
        space += equ[i];
        space += ": ";
        space += label[i];
        space += " -                ";
        mvprintw(4 + i, 20, space.c_str());
    }

    if (d->thepc->equipment.size() > 0) {
        for (i = 0; i < 12; i++) {

            if (d->thepc->equipment[i]) {
                mvprintw(4 + i, 33, d->thepc->equipment[i]->get_name());
            }
        }
    } else {
        for (i = 0; i < 12; i++) {
            mvprintw(4 + i, 33, "none");
        }
    }

    equipment_screen(d, in);
    io_display(d);
}

void equipment_screen(dungeon_t *d, char in) {
    if (in == 'e') {
        mvprintw(2, 20, "ENTER LETTER OF REMOVAL");
        while (1) {
            switch (getch()) {
                case 27:
                    return;
            }
        }
    } else {
        while (1) {
            switch (getch()) {
                case 27:
                    return;
                case 'a':
                    if (d->thepc->remove(0)) {
                        return;
                    }
                    break;
                case 'b':
                    if (d->thepc->remove(1)) {
                        return;
                    }
                    break;
                case 'c':
                    if (d->thepc->remove(2)) {
                        return;
                    }
                    break;
                case 'd':
                    if (d->thepc->remove(3)) {
                        return;
                    }
                    break;
                case 'e':
                    if (d->thepc->remove(4)) {
                        return;
                    }
                    break;
                case 'f':
                    if (d->thepc->remove(5)) {
                        return;
                    }
                    break;
                case 'g':
                    if (d->thepc->remove(6)) {
                        return;
                    }
                    break;
                case 'h':
                    if (d->thepc->remove(7)) {
                        return;
                    }
                    break;
                case 'i':
                    if (d->thepc->remove(8)) {
                        return;
                    }
                    break;
                case 'j':
                    if (d->thepc->remove(9)) {
                        return;
                    }
                    break;
                case 'k':
                    if (d->thepc->remove(10)) {
                        return;
                    }
                    break;
                case 'l':
                    if (d->thepc->remove(11)) {
                        return;
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void io_handle_input(dungeon_t *d) {
    uint32_t fail_code;
    int key;
    fd_set readfs;
    struct timeval tv;
    enum display_mode {
        display_mode_normal,
        display_mode_tunnel,
        display_mode_distance,
        display_mode_hardness,
        display_mode_all
    } dm = display_mode_normal;

    do {
        do {
            FD_ZERO(&readfs);
            FD_SET(STDIN_FILENO, &readfs);

            tv.tv_sec = 0;
            tv.tv_usec = 125000; /* An eigth of a second */

            switch (dm) {
                case display_mode_normal:
                    io_display(d);
                    break;
                case display_mode_tunnel:
                    io_display_tunnel(d);
                    break;
                case display_mode_distance:
                    io_display_distance(d);
                    break;
                case display_mode_hardness:
                    io_display_hardness(d);
                    break;
                case display_mode_all:
                    io_display_all(d);
                    break;
            }
        } while (!select(STDIN_FILENO + 1, &readfs, NULL, NULL, &tv));

        switch (key = getch()) {
            case '7':
            case 'y':
            case KEY_HOME:
                fail_code = move_pc(d, 7);
                break;
            case '8':
            case 'k':
            case KEY_UP:
                fail_code = move_pc(d, 8);
                break;
            case '9':
            case 'u':
            case KEY_PPAGE:
                fail_code = move_pc(d, 9);
                break;
            case '6':
            case 'l':
            case KEY_RIGHT:
                fail_code = move_pc(d, 6);
                break;
            case '3':
            case 'n':
            case KEY_NPAGE:
                fail_code = move_pc(d, 3);
                break;
            case '2':
            case 'j':
            case KEY_DOWN:
                fail_code = move_pc(d, 2);
                break;
            case '1':
            case 'b':
            case KEY_END:
                fail_code = move_pc(d, 1);
                break;
            case '4':
            case 'h':
            case KEY_LEFT:
                fail_code = move_pc(d, 4);
                break;
            case '5':
            case ' ':
            case KEY_B2:
                fail_code = 0;
                break;
            case '>':
                fail_code = move_pc(d, '>');
                break;
            case '<':
                fail_code = move_pc(d, '<');
                break;
            case 'q':
            case 'Q':
                d->quit = 1;
                fail_code = 0;
                break;
            case 'T':
                /* New command.  Display the distances for tunnelers.             */
                io_display_tunnel(d);
                dm = display_mode_tunnel;
                fail_code = 1;
                break;
            case 'D':
                /* New command.  Display the distances for non-tunnelers.         */
                io_display_distance(d);
                dm = display_mode_distance;
                fail_code = 1;
                break;
            case 'H':
                /* New command.  Display the hardnesses.                          */
                io_display_hardness(d);
                dm = display_mode_hardness;
                fail_code = 1;
                break;
            case 's':
                /* New command.  Return to normal display after displaying some   *
                 * special screen.                                                */
                io_display(d);
                fail_code = 1;
                break;
            case 'g':
                /* Teleport the PC to a random place in the dungeon.              */
                io_teleport_pc(d);
                fail_code = 0;
                break;
            case 'm':
                io_list_monsters(d);
                fail_code = 1;
                break;
            case 'w':
                io_list_inventory(d, 'w');
                fail_code = 1;
                break;
            case 'd':
                io_list_inventory(d, 'd');
                fail_code = 1;
                break;
            case 'x':
                io_list_inventory(d, 'x');
                fail_code = 1;
                break;
            case 'I':
                io_list_inventory(d, 'I');
                fail_code = 1;
                break;
            case 't':
                io_list_equipment(d, 't');
                fail_code = 1;
                break;
            case 'i':
                io_list_inventory(d, 'i');
                fail_code = 1;
                break;
            case 'e':
                io_list_equipment(d, 'e');
                fail_code = 1;
                break;
            case 'a':
                io_display_all(d);
                dm = display_mode_all;
                fail_code = 1;
                break;
            default:
                /* Also not in the spec.  It's not always easy to figure out what *
                 * key code corresponds with a given keystroke.  Print out any    *
                 * unhandled key here.  Not only does it give a visual error      *
                 * indicator, but it also gives an integer value that can be used *
                 * for that key in this (or other) switch statements.  Printed in *
                 * octal, with the leading zero, because ncurses.h lists codes in *
                 * octal, thus allowing us to do reverse lookups.  If a key has a *
                 * name defined in the header, you can use the name here, else    *
                 * you can directly use the octal value.                          */
                mvprintw(0, 0, "Unbound key: %#o ", key);
                fail_code = 1;
        }
    } while (fail_code);
}
