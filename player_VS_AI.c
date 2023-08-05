#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sys/time.h>
#include "visuals.h"
#include "objects.h"
#include "lists.h"
#include "barriers.h"
#include "moves.h"
#include "pinning_mechanism.h"
#include "engine.h"
#include "player_VS_AI.h"

ANODE *curr_touched_piece;

void    reset_queues(void){
    pthread_barrier_wait(&barrier_ID_0_bookeeping);
    // All threads delete pieces' move nodes
    // using Unbounded Lock-Free delete function (CAS used)
    reset_move_queues();    
    if( get_self_ID() == 0 ){
        season = !season;
        defend_from_check_flag = isInCheck();
        printf("\t\t\t\tIs in check ? %d\n", defend_from_check_flag);
    }
    wait_to_start_new_turn();
}

void    execute_AI(void){
    if( game_state==STATE_AI_TO_MOVE ){
        choose_piece_and_init_moves();
        wait_for_move_generation();  
        
        if( season!=player_colour){
            compare_each_moves_strength();
            pthread_barrier_wait(&barrier_played_move_nq);

            if( get_self_ID() == 0 ){
                // printf("Rounds best move strength = %d\n", rounds_best_move->strength);
                if( is_checkmate() ){
                    printf("[INFO]: Checkmate!\n");
                    game_over = 0; // Let the user decide when to exit
                    game_state = STATE_GAME_OVER;
                    return;
                }
                else{
                    enqueue_played_move();
                    update_arsenal();
                    check_if_game_over();
                    reset_current_move_str();
                    reset_pieces_locks();
                    update_board(rounds_best_move, 0);
                }
            }
        }
        reset_queues();
        game_state = STATE_AI_INIT_PLAYER_MOVES;
    }
    else if( game_state==STATE_AI_INIT_PLAYER_MOVES ){
        choose_piece_and_init_moves();
//////////////////////////////////////////////// 
        compare_each_moves_strength();
////////////////////////////////////////////////////////////////////
        if( get_self_ID()==0 ){
            if( rounds_best_move->strength==0 ){
                game_state = STATE_GAME_OVER;
            }
            else{
                game_state = STATE_CHOOSE_PIECE_TO_MOVE;
            }
        }
        wait_for_move_generation();
    }
    else if( game_state==STATE_RESET_PLAYER_MOVE_POOL ){
        reset_current_move_str();
        reset_pieces_locks();
        reset_queues();
        game_state = STATE_AI_TO_MOVE;
    }
}

void    get_player_colour_choice(int mx, int my){
    int box_x  = COLOUR_OPTION_1_X+OPTION_BOX_SIZE_X;
    int box1_y  = COLOUR_OPTION_1_Y+OPTION_BOX_SIZE_Y;
    int box2_y1 = COLOUR_OPTION_1_Y+OPTION_BOX_SIZE_Y+SPACING;
    int box2_y2 = COLOUR_OPTION_1_Y+2*OPTION_BOX_SIZE_Y+SPACING;

    if( mx>COLOUR_OPTION_1_X && mx<box_x && my>COLOUR_OPTION_1_Y && my<box1_y ){
        player_colour = WHITE_TO_MOVE;
        game_state=STATE_CHOOSE_PIECE_TO_MOVE;
    }
    else if( mx>COLOUR_OPTION_1_X && mx<box_x && my>box2_y1 && my<box2_y2 ){
        player_colour = BLACK_TO_MOVE;
        game_state=STATE_AI_TO_MOVE;
    }
}

void    get_player_piece_choice(int mx, int my){
    int i, j;

    if( mx>0 && my>0 ){
        i = player_colour==WHITE_PIECE ? (mx-BOARD_OFFSET_X)/SQ_SIZE : 7-(mx-BOARD_OFFSET_X)/SQ_SIZE;
        j = player_colour==WHITE_PIECE ? (my-BOARD_OFFSET_Y)/SQ_SIZE : 7-(my-BOARD_OFFSET_Y)/SQ_SIZE;
        if( i>=0 && i<8 && j>=0 && j<8 ){
            if( mx>BOARD_OFFSET_X && mx<BOARD_OFFSET_X+8*SQ_SIZE &&
                my>BOARD_OFFSET_Y && my<BOARD_OFFSET_Y+8*SQ_SIZE   ){
                if( (board[i][j]->piece>0 && player_colour==WHITE_TO_MOVE) ||
                    (board[i][j]->piece<0 && player_colour==BLACK_TO_MOVE)   ){
                        board[i][j]->colouring = TOUCHED;
                        game_state = STATE_HIGHLIGHT_SQUARE_TO_MOVE_TO;
                }
            }    
        }
    }
}

int    mark_squares_from_move_pool(int x, int y){
    ANODE *p;
    MNODE *ptr;
    int i, j;
    int piece_has_no_moves = 1;

    if( player_colour==WHITE_TO_MOVE ){
        p = w_arsenal;
    }
    else{
        p = b_arsenal;
    }
// Find the piece that the player touched
    while( p!=NULL ){
        if( p->coords[0]==x && p->coords[1]==y ){
            break;
        }
        p = p->next;
    }
    curr_touched_piece = p;
// For all the possible moves the piece can make, mark the squares
    if( p!=NULL ){
        ptr = p->mhead;
        if( p->mhead==p->mtail ){
            // Piece has no moves
            printf("[INFO]: Piece at (%d, %d) has no moves! (season = %d)\n", x,y, season);
            board[x][y]->colouring = NONE;
            game_state = STATE_CHOOSE_PIECE_TO_MOVE;
        }
        else{
            while( ptr!=p->mtail ){
                i = ptr->new_c[0];
                j = ptr->new_c[1];
                if( ptr->strength>0 ){
                    board[i][j]->colouring = TO_MOVE;
                    piece_has_no_moves = 0;
                }
                ptr=ptr->next;
            }
            return piece_has_no_moves;
        }
    }
    else{
        printf("[ERROR]: Piece at (%d,%d) not found! (Funct: mark_from_pool)\n", x,y);
        return piece_has_no_moves;
    }
}

void    reset_board_colouring(void){
    int i,j;
    for( i=0; i<8; i++ ){
        for( j=0; j<8; j++ ){
            board[i][j]->colouring = NONE;
        }
    }
}

void    highlight_available_squares(void){
    int i, j;
    int found = 0;
    int piece_has_no_moves;

    for( i=0; i<8; i++ ){
        for( j=0; j<8; j++ ){
            if( board[i][j]->colouring==TOUCHED ){
                piece_has_no_moves = mark_squares_from_move_pool(i,j);
                game_state = STATE_EXECUTE_PLAYED_MOVE;
                found = 1;
                break;
            }
        }
    }
    if( found==0 || piece_has_no_moves ){
        reset_board_colouring();
        game_state = STATE_CHOOSE_PIECE_TO_MOVE;
    }
}

ANODE  *find_players_chosen_piece(void){
    int i,j;
    int found = 0;
    ANODE *p;
    if( player_colour==WHITE_TO_MOVE){
        p = w_arsenal;
    }
    else{
        p = b_arsenal;
    }

    for( i=0; i<8; i++ ){
        for( j=0; j<8; j++ ){
            if( board[i][j]->colouring==TOUCHED ){
                found = 1;
                break;
            }
        }
        if( found==1 ){
            break;
        }
    }
    if( found==0 ){
        printf("[ERROR]: No piece was touched by the player!\n");
        return NULL;
    }
    else{
        while( p!=NULL ){
            if( p->coords[0]==i && p->coords[1]==j ){
                return p;
            }
            p = p->next;
        }
        if(p==NULL){
            printf("[ERROR]: No piece with coords (%d,%d) in players arsenal!\n", i, j);
        }
    }
}

void    execute_move(int new_x, int new_y){
    ANODE *piece_ptr = find_players_chosen_piece();
    MNODE *mv_node   = malloc(sizeof(MNODE));
    int old_x = piece_ptr->coords[0];
    int old_y = piece_ptr->coords[1];

    mv_node->old_c[0] = old_x;
    mv_node->old_c[1] = old_y;
    mv_node->new_c[0] = new_x;
    mv_node->new_c[1] = new_y;
    mv_node->next = NULL;

    piece_ptr->coords[0] = new_x;
    piece_ptr->coords[1] = new_y;
    update_king_and_rook_flags(mv_node);

    if( board[new_x][new_y]->piece!=0 ){
        remove_from_arsenal(mv_node);
    }
    board[new_x][new_y]->piece = board[old_x][old_y]->piece;
    board[old_x][old_y]->piece = 0;

    if( (piece_ptr->piece==KING || piece_ptr->piece==-KING) && ((old_x-new_x== 2) || (old_x-new_x==-2))){
        update_rook_position_on_castles(new_x);
    }
    reset_board_colouring();
}

void    set_promotion_choice_buttons(int x, int y){
    promotion_panel_location[0] = x;
    promotion_panel_location[1] = y;
}

void    try_to_execute_move(int mx, int my){
    int i,j;
    int promo_x = (mx-BOARD_OFFSET_X)/SQ_SIZE;
    int promo_y = (my-BOARD_OFFSET_Y)/SQ_SIZE;
    int pawn = (player_colour==WHITE_PIECE) ? PAWN : -PAWN;
    int end_of_the_board = (player_colour==WHITE_PIECE) ? 0 : 7;

    if( mx>0 && my>0 ){
        if( mx>0 && my>0 ){
            i = player_colour==WHITE_PIECE ? (mx-BOARD_OFFSET_X)/SQ_SIZE : 7-(mx-BOARD_OFFSET_X)/SQ_SIZE;
            j = player_colour==WHITE_PIECE ? (my-BOARD_OFFSET_Y)/SQ_SIZE : 7-(my-BOARD_OFFSET_Y)/SQ_SIZE;

            if( i>=0 && i<8 && j>=0 && j<8 ){
                if( board[i][j]->colouring==TO_MOVE ){
                    if( curr_touched_piece->piece==pawn && j==end_of_the_board ){
                        set_promotion_choice_buttons(promo_x, promo_y);
                        game_state = STATE_PROMOTION_CHOICE;
                    }
                    else{
                        execute_move(i,j);
                        game_state = STATE_RESET_PLAYER_MOVE_POOL;
                    }
                }
                else{
                    reset_board_colouring();
                    game_state = STATE_CHOOSE_PIECE_TO_MOVE;
                }
            }
            else{
                reset_board_colouring();
                game_state = STATE_CHOOSE_PIECE_TO_MOVE;
            }
        }
        
    }
}

void    execute_promotion(int mx, int my){
    ANODE *p = find_players_chosen_piece();
    int i = (mx-BOARD_OFFSET_X)/SQ_SIZE-promotion_panel_location[0];
    int promoted_pawn_x = player_colour==WHITE_PIECE ? promotion_panel_location[0] : 7-promotion_panel_location[0];
    int promoted_pawn_y = player_colour==WHITE_PIECE ? promotion_panel_location[1] : 7-promotion_panel_location[1];
    int promotion_box_start_x = (promotion_panel_location[0]-1)*SQ_SIZE + BOARD_OFFSET_X;
    int promotion_box_end_x   = (promotion_panel_location[0]+3)*SQ_SIZE + BOARD_OFFSET_X;
    int promotion_box_start_y = promotion_panel_location[1]*SQ_SIZE + BOARD_OFFSET_Y - (SQ_SIZE/2);
    int promotion_box_end_y   = promotion_panel_location[1]*SQ_SIZE + BOARD_OFFSET_Y + (SQ_SIZE/2);

    if( mx<0 ){
        /* return */
    }
    else if( mx<promotion_box_start_x || mx>promotion_box_end_x || my<promotion_box_start_y || my>promotion_box_end_y ){
        reset_board_colouring();
        game_state = STATE_CHOOSE_PIECE_TO_MOVE;
    }
    else{
        execute_move(promoted_pawn_x, promoted_pawn_y);

        if( i==-1 ){
            p->piece = (player_colour==WHITE_PIECE) ? ROOK : -ROOK;
            board[promoted_pawn_x][promoted_pawn_y]->piece = (player_colour==WHITE_PIECE) ? ROOK : -ROOK;
        }
        else if( i==0 ){
            p->piece = (player_colour==WHITE_PIECE) ? QUEEN : -QUEEN;
            board[promoted_pawn_x][promoted_pawn_y]->piece = (player_colour==WHITE_PIECE) ? QUEEN : -QUEEN;
        }
        else if( i==1 ){
            p->piece = (player_colour==WHITE_PIECE) ? KNIGHT : -KNIGHT;
            board[promoted_pawn_x][promoted_pawn_y]->piece = (player_colour==WHITE_PIECE) ? KNIGHT : -KNIGHT;
        }
        else if(i==2){
            p->piece = (player_colour==WHITE_PIECE) ? BISHOP : -BISHOP;
            board[promoted_pawn_x][promoted_pawn_y]->piece = (player_colour==WHITE_PIECE) ? BISHOP : -BISHOP;
        }
        else{
            fprintf(stderr, "[ERROR]: Promotion choice x=%d out of bounds\n", i);
        }
        game_state = STATE_RESET_PLAYER_MOVE_POOL;
    }
}

void    execute_player_action(void){
    int mouse_x = -1;
    int mouse_y = -1;
    
    if( N==1 || get_self_ID()==0 ){ // N = number of threads
        if( al_get_next_event(event_queue, &event) ){
            if( event.type==ALLEGRO_EVENT_DISPLAY_CLOSE ){
                al_destroy_display(display);
                game_over = 1;
            }
            else if( event.type==ALLEGRO_EVENT_MOUSE_BUTTON_UP ){
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            }

            if( game_state==STATE_COLOUR_CHOICE ){
                get_player_colour_choice(mouse_x, mouse_y);
            }
            else if( game_state==STATE_CHOOSE_PIECE_TO_MOVE ){
                get_player_piece_choice(mouse_x, mouse_y);
            }
            else if( game_state==STATE_HIGHLIGHT_SQUARE_TO_MOVE_TO ){
                highlight_available_squares();
            }
            else if( game_state==STATE_EXECUTE_PLAYED_MOVE ){
                try_to_execute_move(mouse_x, mouse_y);
            }
            else if( game_state==STATE_PROMOTION_CHOICE ){
                execute_promotion(mouse_x, mouse_y);
            }
            else if( game_state== STATE_GAME_OVER ){
                if( event.type==ALLEGRO_EVENT_MOUSE_BUTTON_UP ){
                    al_destroy_display(display);
                    game_over = 1;
                }
            }
        }
    }
}

void    game_loop_p_vs_ai(void){
    if( get_self_ID()==0 ){
        v_init();
        choose_piece_and_init_moves();
        compare_each_moves_strength();
        al_start_timer(timer);
    }
    
    while( game_over == 0 ){
        execute_AI();
        execute_player_action();
        v_render();
    }

    pthread_exit(0);
}
