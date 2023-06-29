CC = gcc
FLAGS = -lpthread -g `pkg-config --cflags --libs allegro-5 allegro_primitives-5 allegro_font-5 allegro_image-5 allegro_ttf-5`
BIN = main
SOURCE = main.c visuals.c barriers.c lists.c moves.c objects.c pinning_mechanism.c engine.c player_VS_AI.c


graph:
	$(CC) $(SOURCE) $(FLAGS) -o $(BIN)

all:
	gcc main.c lists.c moves.c pinning_mechanism.c engine.c -lpthread -o main

clean:
	rm main