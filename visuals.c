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


ALLEGRO_COLOR BROWN, WHITE, BLACK, OCHRA, GREY, BLUE, GREEN, RED;
ALLEGRO_DISPLAY     *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER       *timer;
ALLEGRO_FONT        *my_font;
ALLEGRO_EVENT        event;

void    v_init(void){
    al_init();
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    al_set_window_title(display, "Basic Chess Engine");
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    my_font = al_load_font("ArchivoBlackRegular.ttf", 30, 0);
    v_init_colours();

    if( !al_install_mouse() ){
        fprintf(stderr, "[ERROR]: Mouse installation failed!\n");
    }
    event_queue = al_create_event_queue();
    timer = al_create_timer( 1.0 / FPS );

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
}

void    v_init_colours(void){
    BROWN = al_map_rgb(255, 128, 0);
    WHITE = al_map_rgb(255, 255, 255);
    BLACK = al_map_rgb(0,   0,   0);
    OCHRA = al_map_rgb(236, 223, 189);
    GREY  = al_map_rgb(88,  88,  88);
    BLUE  = al_map_rgb(20,  255, 255);
    GREEN = al_map_rgb(51,  255, 150);
    RED   = al_map_rgb(255, 0,   30);
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

    x = BOARD_OFFSET_X;
    y = BOARD_OFFSET_Y;
    for(i=0; i<8; i++){
        for(j=0; j<8;j++){
            if( board[i][j]->colouring==TOUCHED ){
                al_draw_rectangle(x+SELECT_BOX_THICKNESS, y+SELECT_BOX_THICKNESS, x+SQ_SIZE-SELECT_BOX_THICKNESS, y+SQ_SIZE-SELECT_BOX_THICKNESS, BLUE, 10.0);
            }
            else if( board[i][j]->colouring==TO_MOVE ){
                al_draw_rectangle(x+SELECT_BOX_THICKNESS, y+SELECT_BOX_THICKNESS, x+SQ_SIZE-SELECT_BOX_THICKNESS, y+SQ_SIZE-SELECT_BOX_THICKNESS, GREEN, 10.0);
            }
            y = y + SQ_SIZE;
        }
        x += SQ_SIZE; 
        y = BOARD_OFFSET_Y;
    }
}

void    v_print_knight(int cx, int cy, ALLEGRO_COLOR colour){
    // lower
    al_draw_triangle(cx+3, cy-5, cx+15, cy+32, cx-8, cy+32, GREY, 2.0);
    al_draw_filled_triangle(cx+3, cy-4, cx+14, cy+31, cx-7, cy+31, colour);
    // upper
    al_draw_triangle(cx-10, cy-30, cx+35, cy+5, cx-10, cy+5, GREY, 2.0);
    al_draw_filled_triangle(cx-9, cy-28, cx+33, cy+4, cx-9, cy+4, colour);
}

void    v_print_bishop(int cx, int cy, ALLEGRO_COLOR colour){
    al_draw_circle(cx, cy, SQ_SIZE/3-5, GREY, 3.0);
    al_draw_filled_circle(cx, cy, SQ_SIZE/3-6, colour);
}

void    v_print_pawn(int cx, int cy, ALLEGRO_COLOR colour){
    al_draw_rectangle(cx-6, cy-11, cx+6, cy+32, GREY, 2.0);
    al_draw_filled_rectangle(cx-5, cy-10, cx+5, cy+31, colour);
    al_draw_circle(cx, cy-10, SQ_SIZE/8+3, GREY, 3.0);
    al_draw_filled_circle(cx, cy-10, SQ_SIZE/8+2, colour);
    al_draw_filled_rectangle(cx-14, cy+25, cx+14, cy+31, colour);
    al_draw_rectangle       (cx-15, cy+26, cx+15, cy+32, GREY, 2.0);
}

void    v_print_rook(int cx, int cy, ALLEGRO_COLOR colour){
    int left = cx - 23;
    int top = cy - 27;
    int right = cx + 23;
    int bottom = cy + 32;

    al_draw_filled_rectangle(left-1, top-1, right-1, bottom-1, colour);
    al_draw_rectangle(left, top, right, bottom, GREY, 2.0);
}

void    v_print_queen(int cx, int cy, ALLEGRO_COLOR colour){
    
    al_draw_filled_rectangle(cx-35, cy-35, cx-20, cy+25, colour);
    al_draw_rectangle(cx-35, cy-35, cx-20, cy+25, GREY, 2.0);
    al_draw_filled_rectangle(cx+20, cy-35, cx+35, cy+25, colour);
    al_draw_rectangle(cx+20, cy-35, cx+35, cy+25, GREY,2.0);

    al_draw_filled_rectangle(cx-8, cy-11, cx+8, cy+32, colour);
    al_draw_rectangle(cx-8, cy-11, cx+8, cy+32, GREY, 2.0);

    al_draw_filled_rectangle(cx-30, cy+32, cx+30, cy+16, colour);
    al_draw_rectangle(cx-30, cy+32, cx+30, cy+16, GREY, 2.0);
}

void    v_print_king(int cx, int cy, ALLEGRO_COLOR colour){
    al_draw_filled_rectangle(cx-6, cy-21, cx+6, cy+21, colour);
    al_draw_rectangle(cx-6, cy-21, cx+6, cy+21, GREY, 2.0);
    al_draw_filled_rectangle(cx-35, cy-28, cx+35, cy-15, colour);
    al_draw_filled_rectangle(cx-35, cy+28, cx+35, cy+15, colour);
    al_draw_filled_rectangle(cx-35, cy-28, cx-22, cy+28, colour);
    al_draw_filled_rectangle(cx+22, cy-28, cx+35, cy+28, colour);
    al_draw_rectangle(cx-35, cy-28, cx+35, cy+28, GREY,2.0);
    al_draw_rectangle(cx-22, cy-15, cx+22, cy+15, GREY,2.0);

    al_draw_filled_rectangle(cx-8, cy-35, cx+8, cy-28, colour);
    al_draw_rectangle(cx-8, cy-35, cx+8, cy-28, GREY, 2.0);
}

void    v_print_pieces(void){
    int i, j, x, y;
    for( i=0; i<8; i++ ){
        for( j=0; j<8; j++ ){
            x = (i+2)*SQ_SIZE+CENTER;
            y = (j+1)*SQ_SIZE+CENTER/5;
            if( board[i][j]->piece==KNIGHT || board[i][j]->piece==-KNIGHT ){
                if( board[i][j]->piece > 0 )
                    v_print_knight(x, y, WHITE);
                else
                    v_print_knight(x, y, BLACK);
            }
            else if( board[i][j]->piece==BISHOP || board[i][j]->piece==-BISHOP ){
                if( board[i][j]->piece > 0 )
                    v_print_bishop(x+10, y-5, WHITE);
                else
                    v_print_bishop(x+10, y-5, BLACK);
            }
            else if( board[i][j]->piece==PAWN || board[i][j]->piece==-PAWN ){
                if( board[i][j]->piece > 0 )
                    v_print_pawn(x+10, y-5, WHITE);
                else
                    v_print_pawn(x+10, y-5, BLACK);
            }
            else if( board[i][j]->piece==ROOK || board[i][j]->piece==-ROOK ){
                if( board[i][j]->piece > 0 )
                    v_print_rook(x+10, y-5, WHITE);
                else
                    v_print_rook(x+10, y-5, BLACK);
            }
            else if( board[i][j]->piece==QUEEN || board[i][j]->piece==-QUEEN ){
                if( board[i][j]->piece > 0 )
                    v_print_queen(x+10, y-5, WHITE);
                else
                    v_print_queen(x+10, y-5, BLACK);
            }
            else if( board[i][j]->piece==KING || board[i][j]->piece==-KING ){
                if( board[i][j]->piece > 0 )
                    v_print_king(x+10, y-5, WHITE);
                else
                    v_print_king(x+10, y-5, BLACK);
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
            }
            al_flip_display();
            al_clear_to_color(BLACK);
        }
    } 
}