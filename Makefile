CC = gcc
FLAGS = -lpthread -g `pkg-config --cflags --libs allegro-5 allegro_primitives-5`
BIN = main
SOURCE = main.c visuals.c lists.c moves.c pinning_mechanism.c


graph:
	$(CC) $(SOURCE) $(FLAGS) -o $(BIN)

all:
	gcc main.c lists.c moves.c -lpthread -o main

clean:
	rm main