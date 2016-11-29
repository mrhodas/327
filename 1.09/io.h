#ifndef IO_H
# define IO_H

#include "object.h"

typedef struct dungeon dungeon_t;

void io_init_terminal(void);
void io_reset_terminal(void);
void io_display(dungeon_t *d);
void io_handle_input(dungeon_t *d);
void io_queue_message(const char *format, ...);
void esc_pause();
void equipment_screen(dungeon_t *d, char in);
void equip_item(dungeon_t *d);
void expunge_item(dungeon_t *d);
void drop_item(dungeon_t *d);
void inspect_item(dungeon_t *d);
void display_object_desc(std::string str);

#endif
