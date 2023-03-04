#ifndef __VISUALS__
#define __VISUALS__

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#define SQ_SIZE              100
#define SCREEN_WIDTH         1200
#define SCREEN_HEIGHT        900
#define FPS                  144

#define BOARD_OFFSET_X       2*SQ_SIZE
#define BOARD_OFFSET_Y       SQ_SIZE/2

#define COLOUR_OPTION_1_X    1.0
#define COLOUR_OPTION_1_Y    1.0
#define OPTION_BOX_SIZE_X    400.0
#define OPTION_BOX_SIZE_Y    150.0
#define OPTION_BOX_THICKNESS 10.0
#define SPACING              15.0
#define SELECT_BOX_THICKNESS 5.0


ALLEGRO_COLOR BROWN, WHITE, BLACK, OCHRA, GREY, BLUE, GREEN, RED;
ALLEGRO_DISPLAY     *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER       *timer;
ALLEGRO_FONT        *my_font;
ALLEGRO_EVENT        event;

enum square_colouring{ NONE=0, TOUCHED=1, TO_MOVE=2 };

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
void    v_render_colour_choices(void);
void    v_render(void);

#endif