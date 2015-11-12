CC=gcc
LIBS=-lSDL2 -lSDL2_image
C_FLAGS=-c

SRC= ./src/
BIN= ./bin/

OBJS += \
./bin/main.o \
./bin/enemy.o \
./bin/player.o

USER_OBJS += \
./src/main.c \
./src/enemy.c \
./src/player.c

all: Submarino

./bin/%.o: ./src/%.c
	@mkdir -p $(@D)
	$(CC) $(C_FLAGS) -o "$@" "$<"

Submarino: $(OBJS) $(USER_OBJS)
	$(CC) $(FLAGS) -o ./bin/main $(OBJS) $(LIBS)

clean:
	rm $(OBJS)

.PHONY= all clean
