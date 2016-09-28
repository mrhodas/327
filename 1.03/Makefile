CFLAGS = -g -Wall -Werror 

OBJS = \
corridor.o \
debug.o \
dungeon.o \
heap.o \
main.o \
room.o \

NAME = rlg327

all: $(NAME)

clean:
	rm -f $(NAME) *.o .DS_Store

$(NAME): $(OBJS) 
	gcc $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c
	gcc $(CFLAGS) -c $<
