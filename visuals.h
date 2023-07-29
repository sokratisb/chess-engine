#ifndef __VISUALS__
#define __VISUALS__

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#define SQ_SIZE              100
#define SCREEN_WIDTH         1200
#define SCREEN_HEIGHT        1000
#define FPS                  60

#define BOARD_OFFSET_X       2*SQ_SIZE
#define BOARD_OFFSET_Y       SQ_SIZE

#define COLOUR_OPTION_1_X    1.0
#define COLOUR_OPTION_1_Y    1.0
#define OPTION_BOX_SIZE_X    400.0
#define OPTION_BOX_SIZE_Y    150.0
#define OPTION_BOX_THICKNESS 10.0
#define SPACING              15.0
#define SELECT_BOX_THICKNESS 5.0


extern ALLEGRO_COLOR BROWN, WHITE, BLACK, OCHRA, GREY, BLUE, GREEN, FIR, RED;
extern ALLEGRO_DISPLAY     *display;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER       *timer;
extern ALLEGRO_FONT        *my_font;
extern ALLEGRO_EVENT        event;

enum square_colouring{ NONE=0, TOUCHED=1, TO_MOVE=2 };
extern int promotion_panel_location[2];

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