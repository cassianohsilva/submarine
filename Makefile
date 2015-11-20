CC=gcc
LIBS=-lSDL2 -lSDL2_image
C_FLAGS=-c
FLAGS = -Wall

SRC= ./src/
BIN= ./bin/

OBJS += \
./bin/main.o \
./bin/enemy.o \
./bin/player.o \
./bin/game.o \
./bin/bullet.o \
./bin/linked_list.o

USER_OBJS += \
./src/main.c \
./src/enemy.c \
./src/player.c \
./src/game.c \
./src/bullet.c \
./src/linked_list.c

all: Submarino

./bin/%.o: ./src/%.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(C_FLAGS) -o "$@" "$<"

Submarino: $(OBJS) $(USER_OBJS)
	$(CC) $(FLAGS) -o ./main $(OBJS) $(LIBS)

clean:
	rm $(OBJS)

.PHONY= all clean
