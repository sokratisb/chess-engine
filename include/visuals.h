#ifndef __VISUALS__
#define __VISUALS__

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>

#define FPS                  60
#define SQ_SIZE              100
#define SCREEN_WIDTH         (5*SQ_SIZE+8*SQ_SIZE+5*SQ_SIZE)
#define SCREEN_HEIGHT        (SQ_SIZE+8*SQ_SIZE+SQ_SIZE)

#define BOARD_OFFSET_X       (5*SQ_SIZE)
#define BOARD_OFFSET_Y       SQ_SIZE

#define GRAVEYARD_X          (BOARD_OFFSET_X/8)
#define GRAVEYARD_Y          BOARD_OFFSET_Y+(SQ_SIZE/2)

#define COLOUR_OPTION_1_X    1.0
#define COLOUR_OPTION_1_Y    1.0
#define OPTION_BOX_SIZE_X    (float)(4.0*SQ_SIZE)
#define OPTION_BOX_SIZE_Y    (float)(1.5*SQ_SIZE)
#define OPTION_BOX_THICKNESS (float)(SQ_SIZE/10.0)
#define SPACING              (float)(0.15*SQ_SIZE)
#define SELECT_BOX_THICKNESS (float)(0.05*SQ_SIZE)
#define FONT_SIZE            (int)(0.3*SQ_SIZE)


extern ALLEGRO_COLOR        BROWN, WHITE, BLACK, OCHRA, GREY, BLUE, GREEN, FIR, RED;
extern ALLEGRO_DISPLAY     *display;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER       *timer;
extern ALLEGRO_FONT        *my_font;
extern ALLEGRO_EVENT        event;

extern ALLEGRO_BITMAP      *WHITE_PAWN_BITMAP;
extern ALLEGRO_BITMAP      *WHITE_KNIGHT_BITMAP;
extern ALLEGRO_BITMAP      *WHITE_BISHOP_BITMAP;
extern ALLEGRO_BITMAP      *WHITE_ROOK_BITMAP;
extern ALLEGRO_BITMAP      *WHITE_QUEEN_BITMAP;
extern ALLEGRO_BITMAP      *WHITE_KING_BITMAP;
extern ALLEGRO_BITMAP      *BLACK_PAWN_BITMAP;
extern ALLEGRO_BITMAP      *BLACK_KNIGHT_BITMAP;
extern ALLEGRO_BITMAP      *BLACK_BISHOP_BITMAP;
extern ALLEGRO_BITMAP      *BLACK_ROOK_BITMAP;
extern ALLEGRO_BITMAP      *BLACK_QUEEN_BITMAP;
extern ALLEGRO_BITMAP      *BLACK_KING_BITMAP;


enum square_colouring{ NONE=0, TOUCHED=1, TO_MOVE=2 };
extern int promotion_panel_location[2];

void    v_init(void);
void    v_init_colours(void);
void    v_print_board(void);
void    v_print_pawn(int cx, int cy, int colour, float size);
void    v_print_knight(int cx, int cy, int colour, float size);
void    v_print_bishop(int cx, int cy, int colour, float size);
void    v_print_rook(int cx, int cy, int colour, float size);
void    v_print_queen(int cx, int cy, int colour, float size);
void    v_print_king(int cx, int cy, int colour, float size);
void    v_print_pieces(void);
void    v_render_colour_choices(void);
void    v_render(void);
void    v_clear_for_game_over(void);

#endif