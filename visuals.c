#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include "engine.h"
#include "visuals.h"
#include "objects.h"
#include "player_VS_AI.h"


ALLEGRO_COLOR BROWN, WHITE, BLACK, OCHRA, GREY, BLUE, GREEN, FIR, RED;
ALLEGRO_DISPLAY     *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER       *timer;
ALLEGRO_FONT        *my_font;
ALLEGRO_EVENT        event;

ALLEGRO_BITMAP      *WHITE_PAWN_BITMAP;
ALLEGRO_BITMAP      *WHITE_KNIGHT_BITMAP;
ALLEGRO_BITMAP      *WHITE_BISHOP_BITMAP;
ALLEGRO_BITMAP      *WHITE_ROOK_BITMAP;
ALLEGRO_BITMAP      *WHITE_QUEEN_BITMAP;
ALLEGRO_BITMAP      *WHITE_KING_BITMAP;
ALLEGRO_BITMAP      *BLACK_PAWN_BITMAP;
ALLEGRO_BITMAP      *BLACK_KNIGHT_BITMAP;
ALLEGRO_BITMAP      *BLACK_BISHOP_BITMAP;
ALLEGRO_BITMAP      *BLACK_ROOK_BITMAP;
ALLEGRO_BITMAP      *BLACK_QUEEN_BITMAP;
ALLEGRO_BITMAP      *BLACK_KING_BITMAP;

int promotion_panel_location[2];

void    v_init(void){
    al_init();
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    al_set_window_title(display, "Basic Chess Engine");
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    my_font = al_load_font("assets/fonts/ArchivoBlackRegular.ttf", 30, 0);
    v_init_colours();

    if( !al_install_mouse() ){
        fprintf(stderr, "[ERROR]: Mouse installation failed!\n");
    }
    event_queue = al_create_event_queue();
    timer = al_create_timer( 1.0 / FPS );

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    promotion_panel_location[0] = -1;
    promotion_panel_location[1] = -1;

    WHITE_PAWN_BITMAP    = al_load_bitmap("assets/sprites/white_pawn.png");
    WHITE_KNIGHT_BITMAP  = al_load_bitmap("assets/sprites/white_knight.png");
    WHITE_BISHOP_BITMAP  = al_load_bitmap("assets/sprites/white_bishop.png");
    WHITE_ROOK_BITMAP    = al_load_bitmap("assets/sprites/white_rook.png");
    WHITE_QUEEN_BITMAP   = al_load_bitmap("assets/sprites/white_queen.png");
    WHITE_KING_BITMAP    = al_load_bitmap("assets/sprites/white_king.png");
    BLACK_PAWN_BITMAP    = al_load_bitmap("assets/sprites/black_pawn.png");
    BLACK_KNIGHT_BITMAP  = al_load_bitmap("assets/sprites/black_knight.png");
    BLACK_BISHOP_BITMAP  = al_load_bitmap("assets/sprites/black_bishop.png");
    BLACK_ROOK_BITMAP    = al_load_bitmap("assets/sprites/black_rook.png");
    BLACK_QUEEN_BITMAP   = al_load_bitmap("assets/sprites/black_queen.png");
    BLACK_KING_BITMAP    = al_load_bitmap("assets/sprites/black_king.png");
}

void    v_init_colours(void){
    BROWN = al_map_rgb(255, 128,   0);
    WHITE = al_map_rgb(255, 255, 255);
    BLACK = al_map_rgb(  0,   0,   0);
    OCHRA = al_map_rgb(236, 223, 189);
    GREY  = al_map_rgb( 88,  88,  88);
    BLUE  = al_map_rgb( 20, 255, 255);
    GREEN = al_map_rgb( 51, 255, 150);
    FIR   = al_map_rgb(  0, 155,   0);
    RED   = al_map_rgb(255,   0,  30);
}

void    v_print_board(void){
    int i, j;
    int x = BOARD_OFFSET_X;
    int y = BOARD_OFFSET_Y;
    int colour = 0;

    for(i=0; i<8; i++){
        for(j=0; j<8;j++){
            if( colour == 0 ){
                al_draw_filled_rectangle(x, y, x + SQ_SIZE, y + SQ_SIZE, OCHRA);
                colour = 1;
            }
            else{
                al_draw_filled_rectangle(x, y, x + SQ_SIZE, y + SQ_SIZE, BROWN);
                colour = 0;
            }
            x = x + SQ_SIZE;
        }
        if (colour == 0) 
            colour = 1; 
        else
            colour = 0; 
   
        x = 2*SQ_SIZE; 
        y = SQ_SIZE + y; 
    }

    for(i=0; i<8; i++){
        x = player_colour==WHITE_PIECE ? i*SQ_SIZE+BOARD_OFFSET_X : (7-i)*SQ_SIZE+BOARD_OFFSET_X;
        for(j=0; j<8;j++){
            y = player_colour==WHITE_PIECE ? j*SQ_SIZE+BOARD_OFFSET_Y : (7-j)*SQ_SIZE+BOARD_OFFSET_Y;
            if( board[i][j]->colouring==TOUCHED ){
                al_draw_rectangle(x+SELECT_BOX_THICKNESS, y+SELECT_BOX_THICKNESS, x+SQ_SIZE-SELECT_BOX_THICKNESS, y+SQ_SIZE-SELECT_BOX_THICKNESS, BLUE, 10.0);
            }
            else if( board[i][j]->colouring==TO_MOVE ){
                al_draw_rectangle(x+SELECT_BOX_THICKNESS, y+SELECT_BOX_THICKNESS, x+SQ_SIZE-SELECT_BOX_THICKNESS, y+SQ_SIZE-SELECT_BOX_THICKNESS, GREEN, 10.0);
            }
        }

    }
}

void    v_print_knight(int cx, int cy, int colour){
    ALLEGRO_BITMAP *curr_bitmap = colour==WHITE_PIECE ? WHITE_KNIGHT_BITMAP : BLACK_KNIGHT_BITMAP;

    al_draw_scaled_bitmap(
        curr_bitmap, 
        0, 0,
        al_get_bitmap_width(curr_bitmap), al_get_bitmap_width(curr_bitmap),
        cx, cy,
        SQ_SIZE, SQ_SIZE,
        0
    );

}

void    v_print_bishop(int cx, int cy, int colour){
    ALLEGRO_BITMAP *curr_bitmap = colour==WHITE_PIECE ? WHITE_BISHOP_BITMAP : BLACK_BISHOP_BITMAP;

    al_draw_scaled_bitmap(
        curr_bitmap, 
        0, 0,
        al_get_bitmap_width(curr_bitmap), al_get_bitmap_width(curr_bitmap),
        cx, cy,
        SQ_SIZE, SQ_SIZE,
        0
    );
}

void    v_print_pawn(int cx, int cy, int colour){
    ALLEGRO_BITMAP *curr_bitmap = colour==WHITE_PIECE ? WHITE_PAWN_BITMAP : BLACK_PAWN_BITMAP;

    al_draw_scaled_bitmap(
        curr_bitmap, 
        0, 0,
        al_get_bitmap_width(curr_bitmap), al_get_bitmap_width(curr_bitmap),
        cx, cy,
        SQ_SIZE, SQ_SIZE,
        0
    );
}

void    v_print_rook(int cx, int cy, int colour){
    ALLEGRO_BITMAP *curr_bitmap = colour==WHITE_PIECE ? WHITE_ROOK_BITMAP : BLACK_ROOK_BITMAP;

    al_draw_scaled_bitmap(
        curr_bitmap, 
        0, 0,
        al_get_bitmap_width(curr_bitmap), al_get_bitmap_width(curr_bitmap),
        cx, cy,
        SQ_SIZE, SQ_SIZE,
        0
    );
}

void    v_print_queen(int cx, int cy, int colour){
    ALLEGRO_BITMAP *curr_bitmap = colour==WHITE_PIECE ? WHITE_QUEEN_BITMAP : BLACK_QUEEN_BITMAP;

    al_draw_scaled_bitmap(curr_bitmap, 
                          0, 0,
                          al_get_bitmap_width(curr_bitmap), al_get_bitmap_width(curr_bitmap),
                          cx, cy,
                          SQ_SIZE, SQ_SIZE,
                          0 );
}

void    v_print_king(int cx, int cy, int colour){
    ALLEGRO_BITMAP *curr_bitmap = colour==WHITE_PIECE ? WHITE_KING_BITMAP : BLACK_KING_BITMAP;

    al_draw_scaled_bitmap(
        curr_bitmap, 
        0, 0,
        al_get_bitmap_width(curr_bitmap), al_get_bitmap_width(curr_bitmap),
        cx, cy,
        SQ_SIZE, SQ_SIZE,
        0
    );
}

void    v_print_pieces(void){
    int i, j, x, y;
    for( i=0; i<8; i++ ){
        for( j=0; j<8; j++ ){
            x = player_colour==WHITE_PIECE ? (i+2)*SQ_SIZE : (7-i+2)*SQ_SIZE;
            y = player_colour==WHITE_PIECE ? (j+1)*SQ_SIZE : (7-j+1)*SQ_SIZE;
            if( board[i][j]->piece==KNIGHT || board[i][j]->piece==-KNIGHT ){
                if( board[i][j]->piece > 0 )
                    v_print_knight(x, y, WHITE_PIECE);
                else
                    v_print_knight(x, y, BLACK_PIECE);
            }
            else if( board[i][j]->piece==BISHOP || board[i][j]->piece==-BISHOP ){
                if( board[i][j]->piece > 0 )
                    v_print_bishop(x, y, WHITE_PIECE);
                else
                    v_print_bishop(x, y, BLACK_PIECE);
            }
            else if( board[i][j]->piece==PAWN || board[i][j]->piece==-PAWN ){
                if( board[i][j]->piece > 0 )
                    v_print_pawn(x, y, WHITE_PIECE);
                else
                    v_print_pawn(x, y, BLACK_PIECE);
            }
            else if( board[i][j]->piece==ROOK || board[i][j]->piece==-ROOK ){
                if( board[i][j]->piece > 0 )
                    v_print_rook(x, y, WHITE_PIECE);
                else
                    v_print_rook(x, y, BLACK_PIECE);
            }
            else if( board[i][j]->piece==QUEEN || board[i][j]->piece==-QUEEN ){
                if( board[i][j]->piece > 0 )
                    v_print_queen(x, y, WHITE_PIECE);
                else
                    v_print_queen(x, y, BLACK_PIECE);
            }
            else if( board[i][j]->piece==KING || board[i][j]->piece==-KING ){
                if( board[i][j]->piece > 0 )
                    v_print_king(x, y, WHITE_PIECE);
                else
                    v_print_king(x, y, BLACK_PIECE);
            }
        }
    }
}

void    v_render_colour_choices(void){
    al_draw_rectangle( COLOUR_OPTION_1_X, COLOUR_OPTION_1_Y, 
                       COLOUR_OPTION_1_X+OPTION_BOX_SIZE_X, COLOUR_OPTION_1_Y+OPTION_BOX_SIZE_Y,
                       GREY,
                       OPTION_BOX_THICKNESS);
    al_draw_text(my_font,
                 WHITE,
                 (COLOUR_OPTION_1_X+OPTION_BOX_SIZE_X)/2, 
                 (COLOUR_OPTION_1_Y+OPTION_BOX_SIZE_Y-40)/2,
                 ALLEGRO_ALIGN_CENTRE,
                 "WHITE");
    al_draw_rectangle( COLOUR_OPTION_1_X, COLOUR_OPTION_1_Y+OPTION_BOX_SIZE_Y+SPACING, 
                       COLOUR_OPTION_1_X+OPTION_BOX_SIZE_X, COLOUR_OPTION_1_Y+2*OPTION_BOX_SIZE_Y+SPACING,
                       GREY,
                       OPTION_BOX_THICKNESS);
    al_draw_text(my_font,
                 WHITE,
                 (COLOUR_OPTION_1_X+OPTION_BOX_SIZE_X)/2, 
                  COLOUR_OPTION_1_Y+(3*OPTION_BOX_SIZE_Y/2),
                 ALLEGRO_ALIGN_CENTRE,
                 "BLACK");
}

void    v_render_board(void){
    v_print_board();
    v_print_pieces();
}

void    v_render_end_message(void){
    if(rounds_best_move->strength==0 && defend_from_check_flag){
        al_draw_text(my_font,
                 RED,
                 SCREEN_WIDTH/2, 
                 SCREEN_HEIGHT-(SQ_SIZE/2),
                 ALLEGRO_ALIGN_CENTER,
                 "CHECKMATE");
    }
    else{
        al_draw_text(my_font,
                RED,
                SCREEN_WIDTH/2, 
                SCREEN_HEIGHT-(SQ_SIZE/2),
                ALLEGRO_ALIGN_CENTER,
                "STALEMATE");
    }

}

void    v_show_promotion_options(void){
    int i;
    ALLEGRO_COLOR promotion_piece_colour = player_colour==WHITE_PIECE ? WHITE : BLACK;
    int promo_piece_colour = player_colour==WHITE_PIECE ? WHITE_PIECE : BLACK_PIECE;
    int x = promotion_panel_location[0]*SQ_SIZE + BOARD_OFFSET_X - 1*SQ_SIZE;
    int y = promotion_panel_location[1]*SQ_SIZE + BOARD_OFFSET_Y - (SQ_SIZE/2);

// Four different promotion choices
    al_draw_filled_rectangle(x, y, x+SQ_SIZE, y+SQ_SIZE, FIR);
    al_draw_rectangle( x, y, x+SQ_SIZE, y+SQ_SIZE, GREY, OPTION_BOX_THICKNESS/2);
    v_print_rook(x, y, promo_piece_colour);
    
    x+=SQ_SIZE;

    al_draw_filled_rectangle(x, y, x+SQ_SIZE, y+SQ_SIZE, FIR);
    al_draw_rectangle( x, y, x+SQ_SIZE, y+SQ_SIZE, GREY, OPTION_BOX_THICKNESS/2);
    v_print_queen(x, y, promo_piece_colour);

    x+=SQ_SIZE;

    al_draw_filled_rectangle(x, y, x+SQ_SIZE, y+SQ_SIZE, FIR);
    al_draw_rectangle( x, y, x+SQ_SIZE, y+SQ_SIZE, GREY, OPTION_BOX_THICKNESS/2);
    v_print_knight(x, y, promo_piece_colour);

    x+=SQ_SIZE;

    al_draw_filled_rectangle(x, y, x+SQ_SIZE, y+SQ_SIZE, FIR);
    al_draw_rectangle( x, y, x+SQ_SIZE, y+SQ_SIZE, GREY, OPTION_BOX_THICKNESS/2);
    v_print_bishop(x, y, promo_piece_colour);
}

void    v_render(void){
    if( event.type==ALLEGRO_EVENT_TIMER ){
        if( get_self_ID()==0 ){
            if( game_state==STATE_COLOUR_CHOICE ){
                v_render_colour_choices();
            }
            else if( game_state==STATE_GAME_OVER ){
                v_render_board();
                v_render_end_message();
            }
            else{
                v_render_board();
                if( game_state==STATE_PROMOTION_CHOICE ){
                    v_show_promotion_options();
                }
            }
            al_flip_display();
            al_clear_to_color(BLACK);
        }
    } 
}

void    v_clear_for_game_over(void){
    al_destroy_display(display);
    al_destroy_bitmap(WHITE_PAWN_BITMAP);
    al_destroy_bitmap(WHITE_KNIGHT_BITMAP);
    al_destroy_bitmap(WHITE_BISHOP_BITMAP);
    al_destroy_bitmap(WHITE_ROOK_BITMAP);
    al_destroy_bitmap(WHITE_QUEEN_BITMAP);
    al_destroy_bitmap(WHITE_KING_BITMAP);
    al_destroy_bitmap(BLACK_PAWN_BITMAP);
    al_destroy_bitmap(BLACK_KNIGHT_BITMAP);
    al_destroy_bitmap(BLACK_BISHOP_BITMAP);
    al_destroy_bitmap(BLACK_ROOK_BITMAP);
    al_destroy_bitmap(BLACK_QUEEN_BITMAP);
    al_destroy_bitmap(BLACK_KING_BITMAP);
}