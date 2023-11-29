CC = gcc
FLAGS = -lpthread -g `pkg-config --cflags --libs allegro-5 allegro_primitives-5 allegro_font-5 allegro_image-5 allegro_ttf-5`
BIN = chess
SOURCE = src/main.c src/visuals.c src/barriers.c src/lists.c src/moves.c src/objects.c src/pinning_mechanism.c src/engine.c src/player_VS_AI.c


all:
	$(CC) $(SOURCE) $(FLAGS) -o $(BIN)

clean:
	rm chess
