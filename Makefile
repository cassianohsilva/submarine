CC=gcc
LIBS=-lSDL2 -lSDL2_image
CFLAGS=-c

SRC= ./src/
BIN= ./bin/

all: Submarino

player.o: $(SRC)player.c
	$(CC) $(CFLAGS) -o $(BIN)player.o  $(SRC)player.c

main.o: $(SRC)main.c
	$(CC) $(CFLAGS) -o $(BIN)main.o  $(SRC)main.c

Submarino: player.o main.o
	$(CC) -o $(BIN)main $(BIN)player.o $(BIN)main.o $(LIBS)


clean:
	rm $(BIN)*.o

.PHONY= all clean
