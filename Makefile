CC=gcc
LIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
C_FLAGS=-c
FLAGS = -Wall

SRC= ./src/
BIN= ./bin/

OBJS += \
./bin/enemy.o \
./bin/player.o \
./bin/game.o \
./bin/bullet.o \
./bin/linked_list.o \
./bin/timer.o \
./bin/main.o

USER_OBJS += \
./src/enemy.c \
./src/player.c \
./src/game.c \
./src/bullet.c \
./src/linked_list.c \
./src/timer.c \
./src/main.c

all: Submarine

./bin/%.o: ./src/%.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(C_FLAGS) -o "$@" "$<"

Submarine: $(OBJS) $(USER_OBJS)
	$(CC) $(FLAGS) -o ./Submarine $(OBJS) $(LIBS)

clean:
	rm $(OBJS)

.PHONY= all clean
