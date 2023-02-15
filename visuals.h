#ifndef __VISUALS__
#define __VISUALS__

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#define SIZE 100
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900


ALLEGRO_COLOR BROWN, WHITE, BLACK, OCHRA, GREY;

void    v_init(void);
void    v_init_colours(void);
void    v_print_board(void);
void    v_print_pawn(int cx, int cy, ALLEGRO_COLOR colour);
void    v_print_knight(int cx, int cy, ALLEGRO_COLOR colour);
void    v_print_bishop(int cx, int cy, ALLEGRO_COLOR colour);
void    v_print_rook(int cx, int cy, ALLEGRO_COLOR colour);
void    v_print_queen(int cx, int cy, ALLEGRO_COLOR colour);
void    v_print_king(int cx, int cy, ALLEGRO_COLOR colour);
void    v_print_pieces(void);

#endif