#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sys/time.h>
#include "engine.h"
#include "visuals.h"
#include "objects.h"
#include "lists.h"
#include "barriers.h"
#include "moves.h"
#include "pinning_mechanism.h"

#define MAXT 200

int N;
pthread_t threadIDs[MAXT];
int player_colour;
int game_state;


void    put_pieces_to_initial_positions(void){
    int i ,j;
    for( i=0; i<8; i++ ){
        for( j=0; j<8; j++ ){
            if(j == 6){
                board[i][j]->piece =  PAWN;
            }
            else if(j == 1){
                board[i][j]->piece = -PAWN;
            }
            else
                board[i][j]->piece = EMPTY;
        }
    }
    board[0][0]->piece = -ROOK; board[7][0]->piece = -ROOK;
    board[0][7]->piece =  ROOK; board[7][7]->piece =  ROOK;
    board[1][0]->piece = -KNIGHT; board[6][0]->piece = -KNIGHT;
    board[1][7]->piece =  KNIGHT; board[6][7]->piece =  KNIGHT;
    board[2][0]->piece = -BISHOP; board[5][0]->piece = -BISHOP;
    board[2][7]->piece =  BISHOP; board[5][7]->piece =  BISHOP;
    board[3][0]->piece =  -QUEEN; board[3][7]->piece = QUEEN;
    board[4][0]->piece =  -KING;  board[4][7]->piece = KING;
}

void    assign_init_positions_and_arsenals(void){
    int i,j;
    int c[2];
    for( i=0; i<8; i++ ){
        for( j=0; j<8; j++ ){
            if(j == 6){
                board[i][j]->piece =  PAWN;
                ar_insert(PAWN, i, j, WHITE_TO_MOVE);
            }
            else if(j == 1){
                board[i][j]->piece = -PAWN;
                ar_insert(-PAWN, i, j, BLACK_TO_MOVE);
            }
            else
                board[i][j]->piece = EMPTY;
            board[i][j]->colouring = NONE;
        }
    }
    board[0][0]->piece = -ROOK; board[7][0]->piece = -ROOK;
    board[0][7]->piece =  ROOK; board[7][7]->piece =  ROOK;
    ar_insert(-ROOK, 0, 0, BLACK_TO_MOVE);
    ar_insert(-ROOK, 7, 0, BLACK_TO_MOVE);
    ar_insert(ROOK, 0, 7, WHITE_TO_MOVE);
    ar_insert(ROOK, 7, 7, WHITE_TO_MOVE);

    board[1][0]->piece = -KNIGHT; board[6][0]->piece = -KNIGHT;
    board[1][7]->piece =  KNIGHT; board[6][7]->piece =  KNIGHT;
    ar_insert(-KNIGHT, 1, 0, BLACK_TO_MOVE);
    ar_insert(-KNIGHT, 6, 0, BLACK_TO_MOVE);
    ar_insert(KNIGHT, 1, 7, WHITE_TO_MOVE);
    ar_insert(KNIGHT, 6, 7, WHITE_TO_MOVE);

    board[2][0]->piece = -BISHOP; board[5][0]->piece = -BISHOP;
    board[2][7]->piece =  BISHOP; board[5][7]->piece =  BISHOP;
    ar_insert(-BISHOP, 2, 0, BLACK_TO_MOVE);
    ar_insert(-BISHOP, 5, 0, BLACK_TO_MOVE);
    ar_insert(BISHOP, 2, 7, WHITE_TO_MOVE);
    ar_insert(BISHOP, 5, 7, WHITE_TO_MOVE);

    board[3][0]->piece =  -QUEEN; board[3][7]->piece = QUEEN;
    board[4][0]->piece =  -KING;  board[4][7]->piece = KING;
    ar_insert(-QUEEN, 3, 0, BLACK_TO_MOVE);
    ar_insert(-KING, 4, 0, BLACK_TO_MOVE);
    ar_insert(QUEEN, 3, 7, WHITE_TO_MOVE);
    ar_insert(KING, 4, 7, WHITE_TO_MOVE);
}

void    remove_from_arsenal(MNODE *p){
    int i = p->new_c[0];
    int j = p->new_c[1];
    int result;

    if( board[i][j]->piece < 0 ){
        result = ar_delete( b_arsenal,i, j);
        if( result == 0 )
            printf("\tCaptured piece deletion failed\n");
    }
    else if( board[i][j]->piece > 0 ){
        result = ar_delete( w_arsenal,i, j);
        if( result == 0 )
            printf("\tCaptured piece deletion failed\n");
    }
}

void    update_board(MNODE *p, int showcase){
    int old_x = p->old_c[0];
    int old_y = p->old_c[1];
    int new_x = p->new_c[0];
    int new_y = p->new_c[1];
    int end_of_the_board = season==WHITE_TO_MOVE ? 0 : 7;
    ANODE *ptr = season==WHITE_TO_MOVE ? w_arsenal : b_arsenal;

    if( !showcase )
        remove_from_arsenal(p);
    if( ((board[old_x][old_y]->piece==PAWN) || (board[old_x][old_y]->piece==-PAWN)) && (new_y==end_of_the_board) ){
        while( ptr->coords[0]!=new_x || ptr->coords[1]!=new_y ){
            ptr = ptr->next;
        }
        if( ptr==NULL ){
            fprintf("[ERROR]: Funct: update_board >> no piece with coords [%d,%d].\n", new_x, new_y);
        }
        else{
            board[new_x][new_y]->piece = ptr->piece;
            board[old_x][old_y]->piece = 0;
        }
    }
    else{
        board[new_x][new_y]->piece = board[old_x][old_y]->piece;
        board[old_x][old_y]->piece = 0;
    }
}

void    init_board(void){
    int i,j;
    for( i=0; i<8; i++ ){
        for( j =0; j<8; j++ ){
            board[i][j] = malloc(sizeof(SQUARE));
        }
    }
    assign_init_positions_and_arsenals();
}


int    get_self_ID(void){
    int i;
    pthread_t real_ID = pthread_self();
    for( i=0; i<MAXT; i++){
        if( real_ID == threadIDs[i] )
            return i;
    }
}

void    create_piece_move_pool(ANODE *p){
    int piece = p->piece;
    if( (piece == PAWN) || (piece == -PAWN) ){
        printf("Creating pawn moves season = %d\n", season);
        m_create_pawn_moves(p);
    }
    else if( (piece == KNIGHT) || (piece == -KNIGHT) ){
        m_create_knight_moves(p);
    }
    else if( (piece == BISHOP) || (piece == -BISHOP) ){
        m_create_bishop_moves(p);
    }
    else if( (piece == ROOK) || (piece == -ROOK) ){
        m_create_rook_moves(p);
    }
    else if( (piece == QUEEN) || (piece == -QUEEN) ){
        m_create_queen_moves(p);
    }
    else if( (piece == KING) || (piece == -KING) ){
        m_create_king_moves(p);
    }
    else
        printf("\n\tError create_piece_move_pool! Not an actual piece (%d)!\n\n", piece);
}

int     isInCheck(void){
    int king_pos_X, king_pos_Y;
    int dummy_x, dummy_y;
    int check_result = 0;
    int attacker = (season==WHITE_TO_MOVE) ? BLACK_PIECE : WHITE_PIECE;
    MNODE *dummy_move = malloc(sizeof(MNODE));
    FindKingPosition(&king_pos_X, &king_pos_Y);
    dummy_move->old_c[0] = king_pos_X;
    dummy_move->old_c[1] = king_pos_Y;
    dummy_move->new_c[0] = king_pos_X;
    dummy_move->new_c[1] = king_pos_Y;
    check_result = m_look_for_protectors(attacker, dummy_move, &dummy_x, &dummy_y);

    if( check_result!=0 ){
        printf("[INFO]: Check happened! (%d)\n", check_result);
        if( check_result==PAWN || check_result==KNIGHT || check_result==BISHOP ||
            check_result==ROOK || check_result==QUEEN  ||
            check_result==-PAWN || check_result==-KNIGHT || check_result==-BISHOP ||
            check_result==-ROOK || check_result==-QUEEN                               ){
            return SIMPLE_CHECK;
        }
        else{
            // Check by 2 attackers, can only move out of it, not block
            return DOUBLE_CHECK;
        }
    }
}

MNODE    *select_a_move_from_pool(ANODE *p){
    int lock_value;
    MNODE *move = p->mhead;
    while( move != p->mtail ){
        lock_value = atomic_flag_test_and_set(&(move->lock));
        if( lock_value == 0 ){
            return move;
        }
        move = move->next;
    }
    return NULL;
}

void    suggest_current_move(MNODE *move){
    int i, j, str;
    i = move->new_c[0];
    j = move->new_c[1];
    str = move->strength;

    rounds_best_move->strength = str;
    rounds_best_move->old_c[0] = move->old_c[0];
    rounds_best_move->old_c[1] = move->old_c[1];
    rounds_best_move->new_c[0] = i;
    rounds_best_move->new_c[1] = j;
}

int     calc_if_move_blocks_check(MNODE *move, int attacker_x, int attacker_y){
    int blocker_x = move->new_c[0];
    int blocker_y = move->new_c[1];
    int king_x, king_y;
    FindKingPosition(&king_x, &king_y);
    printf("Blocker[%d,%d], Attacker[%d,%d], King[%d,%d]\n", move->new_c[0], move->new_c[1], attacker_x, attacker_y, king_x, king_y);

// Vertical check
    if( king_x==attacker_x ){
        if( blocker_x==king_x ){
            if( king_y>attacker_y ){
                if( blocker_y>=attacker_y && blocker_y<king_y ){
                    return 1;
                }
                else{
                    return 0;
                }
            }
            else{ // king_y<attacker_y
                if( blocker_y<=attacker_y && blocker_y>king_y ){
                    return 1;
                }
                else{
                    return 0;
                }
            }
        }
        else{ // Piece not even in the same collumn as check
            return 0;
        }
    }
// Horizontal check
    else if( king_y==attacker_y ){
        if( blocker_y==king_y ){
            if( king_x>attacker_x ){
                if( blocker_x>=attacker_x && blocker_x<king_x ){
                    return 1;
                }
                else{
                    return 0;
                }
            }
            else{
                if( blocker_x<=attacker_x && blocker_x>king_x ){
                    return 1;
                }
                else{
                    return 0;
                }
            }
        }
        else{
            return 0;
        }
    }
// Check by knight
    else if( board[attacker_x][attacker_y]->piece==KNIGHT || board[attacker_x][attacker_y]->piece==-KNIGHT ){
        if( blocker_x==attacker_x && blocker_y==attacker_y ){
            return 1;
        }
        else{
            return 0;
        }
    }
// Diagonal check
    else{
    // First, perform assertion if check is really diagonal
        if( (king_x>attacker_x && king_y>attacker_y) || (king_x<attacker_x && king_y<attacker_y) ){
            if( king_x+king_y != attacker_x+attacker_y ){
                fprintf(stderr, "[Error]: Diagonal check but attacker (%d, %d) not in same diagonal as king (%d, %d)\n", attacker_x, attacker_y, king_x, king_y);
            }
        }
        else{
            if( king_x-king_y != attacker_x-attacker_y ){
                fprintf(stderr, "[Error]: Diagonal check but attacker (%d, %d) not in same diagonal as king (%d, %d)\n", attacker_x, attacker_y, king_x, king_y);
            }
        }
    
    // Then check if piece can block on the diagonal
        if( (blocker_x+blocker_y==king_x+king_y) && (king_x+king_y==attacker_x+attacker_y) ){
            if( king_x<attacker_x ){
                if( blocker_x>king_x && blocker_x<=attacker_x ){
                    return 1;
                }
                else{
                    return 0;
                }
            }
            else{
                if( blocker_x<king_x && blocker_x>=attacker_x ){
                    return 1;
                }
                else{
                    return 0;
                }
            }
        }
        else if( (blocker_x-blocker_y==king_x-king_y) && (king_x-king_y==attacker_x-attacker_y) ){
            if( king_x<attacker_x ){
                if( blocker_x>king_x && blocker_x<=attacker_x ){
                    return 1;
                }
                else{
                    return 0;
                }
            }
            else{
                if( blocker_x<king_x && blocker_x>=attacker_x ){
                    return 1;
                }
                else{
                    return 0;
                }
            }
        }
        else{
            return 0;
        }
    }
}

int is_covered_by_more_than_one_pieces(int piece_value){
    if( piece_value!=0 ){
        if( piece_value!=KING    || piece_value!=QUEEN   || piece_value!=ROOK  ||
            piece_value!=BISHOP  || piece_value!=KNIGHT  || piece_value!=PAWN  ||
            piece_value!=-KING   || piece_value!=-QUEEN  || piece_value!=-ROOK ||
            piece_value!=-BISHOP || piece_value!=-KNIGHT || piece_value!=-PAWN    ){
                return 1;
            }
        else{
            return 0;
        }
    }
    else{
        return 0;
    }
}

int     compute_moves_strength(MNODE *move){
    int str = 0;
    int bad_move_flag = 0;
    int value = rand();
    int move_x = move->new_c[0];
    int move_y = move->new_c[1];
    int curr_piece = board[ move->old_c[0] ][ move->old_c[1] ]->piece;
    int attacker_x = -1;
    int attacker_y = -1;
    int dummy_x, dummy_y;
    int attacker = (season==WHITE_TO_MOVE) ? BLACK_PIECE : WHITE_PIECE;
    int defender = (season==WHITE_TO_MOVE) ? WHITE_PIECE : BLACK_PIECE;
    int square_covered_by_enemy   = m_look_for_protectors(attacker,move,&dummy_x,&dummy_y);
    int square_covered_by_defender= m_look_for_protectors(defender,move,&dummy_x,&dummy_y);    
    int intrinsic_value = (season==WHITE_TO_MOVE) ? -board[move_x][move_y]->piece : board[move_x][move_y]->piece;
    int curr_piece_can_block = 0;
    if( total_move_count!=0 && defend_from_check_flag){
    // Find the king's attacker coordinates if in check
        int king_x, king_y;
        MNODE *dummy_move = malloc(sizeof(MNODE));
        FindKingPosition(&king_x, &king_y);
        dummy_move->old_c[0] = king_x;
        dummy_move->old_c[1] = king_y;
        dummy_move->new_c[0] = king_x;
        dummy_move->new_c[1] = king_y;
        m_look_for_protectors(attacker, dummy_move, &attacker_x, &attacker_y);
        if( (curr_piece!=KING) && (curr_piece!=-KING) ){
            if( attacker_x >= 0 && attacker_y >= 0 ){
                curr_piece_can_block = calc_if_move_blocks_check(move, attacker_x, attacker_y);
            }
        }
    }

// In double checks kings must move out of it
    if( defend_from_check_flag==DOUBLE_CHECK ){
        if( (curr_piece==KING || curr_piece==-KING) && (square_covered_by_enemy==0) ){
            str = intrinsic_value + MOVE_OUT_OF_CHECK;
        }
        return str;
    }
// In simple check, try to block on defended square/capture attacker or move king if he can capture
// other piece while getting out of check
    else if( defend_from_check_flag==SIMPLE_CHECK ){
        if( (curr_piece==KING || curr_piece==-KING) ){
            if( square_covered_by_enemy==0 ){
                str = intrinsic_value + MOVE_OUT_OF_CHECK; 
            }
        }
        else{ // piece != KING
            if( curr_piece_can_block ){
                if( move_x==attacker_x && move_y==attacker_y ){
                    str = intrinsic_value + CAPTURE_KINGS_ATTACKER;
                }
                else{
                    // The piece will land on a square that can block check
                    if( is_covered_by_more_than_one_pieces(square_covered_by_enemy) ){
                        str = intrinsic_value + BLOCK_CHECK_DOUBLE_ATTACKED;
                    }
                    else{
                        if( square_covered_by_defender ){
                            str = intrinsic_value + BLOCK_CHECK_DEFENDED;
                        }
                        else{
                            str = intrinsic_value + BLOCK_CHECK_UNDEFENDED;
                        }
                    }
                }
            }
            else{ // If the piece cannot block/capture attacker, move is illegal 
                str = 0;
            }
        }
    }
// King not in check
    else{
        if( square_covered_by_enemy ){
            // King should never move to enemy-protected square
            if( (curr_piece==KING) || (curr_piece==-KING) ){
                str = 0;
            }
            // If capturing a defended piece of equal importance
            // assign some value less than capturing a free pawn
            // Also if pawn push to protected square -> go for it
            else if( (board[move_x][move_y]->piece == -curr_piece) ){
                str = TRADE_EQUAL_PIECES +value%RANDOM_FACTOR;
            }
            else if( ( (season==WHITE_TO_MOVE)&&(curr_piece< -board[move_x][move_y]->piece) )  ||
                     ( (season==BLACK_TO_MOVE)&&(-curr_piece< board[move_x][move_y]->piece) )     ){
                str = intrinsic_value + TRADE_UP +value%RANDOM_FACTOR;
            }
            else if( ( (season==WHITE_TO_MOVE)&&(curr_piece> -board[move_x][move_y]->piece) )  ||
                     ( (season==BLACK_TO_MOVE)&&(-curr_piece> board[move_x][move_y]->piece) )     ){
                str = 3;
            }
            else{
                str = MOVE_TO_WHITE_SPACE + value%RANDOM_FACTOR;
            }
        }
        else{
            if( board[move_x][move_y]->piece!=EMPTY ){
                str = intrinsic_value + CAPTURE_UNDEFENDED_PIECE;
            }
            else{
                str = MOVE_TO_WHITE_SPACE + 30+value%RANDOM_FACTOR;
            }
            if( curr_piece==KING || curr_piece==-KING ){
                // Castling
                if( (move->new_c[0]-move->old_c[0]==2) || (move->new_c[0]-move->old_c[0]==-2) ){
                    str = 400;
                }
            }
        }   
    }
    printf("[%d]: Piece=%d\n", total_move_count, curr_piece);
    printf("Attacker[%d,%d], can_block = %d\n", attacker_x, attacker_y, curr_piece_can_block);
    printf("Intrinsic value =%d\n", intrinsic_value);
    printf("Square covered = %d, season = %d, str = %d\n\n", square_covered_by_enemy, season, str);
    return str;
}

void    compare_and_update_rounds_best(MNODE *move){
    int str = compute_moves_strength(move);
    move->strength = str;
    printf("\t\t\t\t\t\t\tstr = %d\n", move->strength);
    if( (str > rounds_best_move->strength) )
        suggest_current_move(move);
}

void    compare_each_moves_strength(void){
    ANODE *piece; MNODE *move;
    if( season == BLACK_TO_MOVE )
        piece = b_arsenal;
    else
        piece = w_arsenal;

    while( piece!=NULL ){
        move = piece->mhead;
        while( move != NULL ){
            move = select_a_move_from_pool(piece);
            if( move != NULL ){
                while( atomic_flag_test_and_set(&(rounds_best_move->lock)) ){;}
                compare_and_update_rounds_best(move);
                rounds_best_move->lock = 0;
            }
        }

        piece = piece->next;
    }
}

void    update_king_and_rook_flags(MNODE *move){
    int i = move->old_c[0];
    int j = move->old_c[1];

    if( i==7 && j==7 && season==WHITE_TO_MOVE && board[i][j]->piece==ROOK && !white_right_rook_has_moved ){
        printf("[INFO]: WHITE RIGHT ROOK MOVED\n");
        white_right_rook_has_moved = 1;
    }
    else if( i==0 && j==7 && season==WHITE_TO_MOVE && board[i][j]->piece==ROOK && !white_left_rook_has_moved ){
        printf("[INFO]: WHITE LEFT ROOK MOVED\n");
        white_right_rook_has_moved = 1;
    }
    else if( i==7 && j==0 && season==BLACK_TO_MOVE && board[i][j]->piece==-ROOK && !black_right_rook_has_moved ){
        printf("[INFO]: BLACK RIGHT ROOK MOVED\n");
        black_right_rook_has_moved = 1;
    }
    else if( i==0 && j==0 && season==BLACK_TO_MOVE && board[i][j]->piece==-ROOK && !black_left_rook_has_moved ){
        printf("[INFO]: BLACK LEFT ROOK MOVED\n");
        black_left_rook_has_moved = 1;
    }
    else if( i==4 && j==7 && season==WHITE_TO_MOVE && board[i][j]->piece==KING && !white_king_has_moved ){
        printf("[INFO]: WHITE KING MOVED\n");
        white_king_has_moved = 1;
    }
    else if( i==4 && j==0 && season==BLACK_TO_MOVE && board[i][j]->piece==-KING && !black_king_has_moved ){
        printf("[INFO]: BLACK KING MOVED\n");
        black_king_has_moved = 1;
    }
}


void    update_rook_position_on_castles(int new_king_x){
    ANODE *ptr;
    if( season == BLACK_TO_MOVE )
        ptr = b_arsenal;
    else
        ptr = w_arsenal;
    while( ptr!=NULL ){
        if( season==BLACK_TO_MOVE ){
            if( (ptr->coords[0]==7 && ptr->coords[1]==0 && (new_king_x==6)) ||
                (ptr->coords[0]==0 && ptr->coords[1]==0 && (new_king_x==2))    ){
                break;
            }
        }
        else{
            if( (ptr->coords[0]==7 && ptr->coords[1]==7 && (new_king_x==6)) ||
                (ptr->coords[0]==0 && ptr->coords[1]==7 && (new_king_x==2))    ){
                break;
            }
        }
        ptr = ptr->next;
    }
    if( ptr == NULL )
        fprintf(stderr, "[ERROR]: Update rook position! Rook not found!\n\n");
    if( new_king_x==6 ){
        ptr->coords[0] = 5;
        if( season==WHITE_TO_MOVE ){
            board[7][7]->piece = 0;
            board[5][7]->piece = ROOK;
        }
        else{
            board[7][0]->piece = 0;
            board[5][0]->piece = -ROOK;
        }
    }
    else{
        ptr->coords[0] = 3;
        if( season==WHITE_TO_MOVE ){
            board[0][7]->piece = 0;
            board[3][7]->piece = ROOK;
        }
        else{
            board[0][0]->piece = 0;
            board[3][0]->piece = -ROOK;
        }
    }
}

void    update_arsenal(void){
    ANODE *ptr;
    int pawn = season==WHITE_TO_MOVE ? PAWN : -PAWN;
    int end_of_the_board = season==WHITE_TO_MOVE ? 0 : 7;

    if( rounds_best_move->strength==0 ){
        fprintf(stderr, "[INFO]: Entered UPDATE ARSENAL while on checkmate!\n");
        return 0;
    }

    if( season == BLACK_TO_MOVE )
        ptr = b_arsenal;
    else
        ptr = w_arsenal;
    while( ptr!=NULL ){
        if(ptr->coords[0]==rounds_best_move->old_c[0] &&
           ptr->coords[1]==rounds_best_move->old_c[1]     )
           break;
        ptr = ptr->next;
    }
    if( ptr == NULL )
        printf("\tUpdate Arsenal Error! %d\n\n", rounds_best_move->strength);
    if( (ptr->piece==KING || ptr->piece==-KING) && ((ptr->coords[0]-rounds_best_move->new_c[0]== 2) || 
                                          (ptr->coords[0]-rounds_best_move->new_c[0]==-2))   ){
        update_rook_position_on_castles(rounds_best_move->new_c[0]);
    }
    update_king_and_rook_flags(rounds_best_move);
    ptr->coords[0] = rounds_best_move->new_c[0];
    ptr->coords[1] = rounds_best_move->new_c[1];
    // Check for AI promotion
    if( (ptr->piece==pawn) && (ptr->coords[1]==end_of_the_board) ){
        int value = rand()%1000;
        if( value<=10 ){
            ptr->piece = season==WHITE_TO_MOVE ? BISHOP : -BISHOP;
        }
        else if( value>10 && value<=30 ){
            ptr->piece = season==WHITE_TO_MOVE ? KNIGHT : -KNIGHT;
        }
        else if( value>30 && value<=40 ){
            ptr->piece = season==WHITE_TO_MOVE ? ROOK : -ROOK;
        }
        else{
            ptr->piece = season==WHITE_TO_MOVE ? QUEEN : -QUEEN;
        }
    }
}

void    check_if_game_over(void){
    total_move_count++;
    if( total_move_count == MAX_MOVES ||
        rounds_best_move->strength > 5000 ||
        rounds_best_move->strength < -5000){
            game_over = 1;
        }
}

void    wait_for_move_generation(void){
    pthread_barrier_wait(&barrier_move_generation);
}

void    wait_to_start_new_turn(void){
    pthread_barrier_wait(&barrier_new_turn);
}

void     choose_piece_and_init_moves(void){
    int TAS_res;
    ANODE *p;
    
    if( season == WHITE_TO_MOVE )
        p = w_arsenal;
    else
        p = b_arsenal;
    
    while(p != NULL){
        while( p!=NULL ){
            TAS_res = atomic_flag_test_and_set( &(p->lock) );
            if( TAS_res == 0 )
                break;
            else
                p = p->next;
        }
        if( p!=NULL ){
            create_piece_move_pool(p);
            p = p->next;
        }
    }
}

void    reset_move_queues(void){
    ANODE *p;
    if( season == WHITE_TO_MOVE )
        p = w_arsenal;
    else
        p = b_arsenal;
    
    while(p != NULL){
        while( p->mhead != p->mtail ){
            m_move_dequeue(p);
        }
        p = p->next;
    }
}

void    reset_current_move_str(void){
    rounds_best_move->strength = 0;
    first_best_move_flag = 0;
}

void    reset_pieces_locks(void){
    ANODE *p;
    if( season == WHITE_TO_MOVE )
        p = w_arsenal;
    else
        p = b_arsenal;
    while( p!=NULL ){
        p->lock = 0;
        p = p->next;
    }
}

void    count_remaining_moves_in_queues(void){
    ANODE *p; MNODE *m;
    int count = 0;
    if( season == WHITE_TO_MOVE )
        p = w_arsenal;
    else
        p = b_arsenal;
    
    while( p!=NULL ){
        count = 0;
        m = p->mhead;
        while( m != p->mtail ){
            count++;
            m = m->next;
        }
        p = p->next;
        if( count != 0 ){
            printf("\tCOUNT !=0 ERROR, MOVE QUEUES NOT DELETED!\n\n");
            game_over = 1;
        }

    }
}

int     is_checkmate(void){
    if( rounds_best_move->strength==0 && defend_from_check_flag ){
        game_over = 1;
        return 1;
    }
    else if( rounds_best_move->strength==0 ){
        printf("[INFO]: Stalemate!\n\n");
        return 1;
    }
    else{
        return 0;
    }
}

void    execute_thread_functs(void){
    unsigned long elapsed_time;
    struct timeval start, finish;

    while( game_over == 0 ){
        choose_piece_and_init_moves();
        wait_for_move_generation(); // barrier    
        
        compare_each_moves_strength();
        pthread_barrier_wait(&barrier_played_move_nq);

        if( get_self_ID() == 0 ){
            // printf("Rounds best move strength = %d\n", rounds_best_move->strength);
            if( is_checkmate() ){
                printf("[INFO]: Game ended!\n");
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
        pthread_barrier_wait(&barrier_ID_0_bookeeping);

    // All threads delete pieces' move nodes
    // using Unbounded Lock-Free delete function (CAS used)
        reset_move_queues();    
        if( get_self_ID() == 0 ){
            count_remaining_moves_in_queues();
            season = !season;
            defend_from_check_flag = isInCheck();
        }

        wait_to_start_new_turn();
    }
    pthread_exit(0);
}

void    init_locks(void){
    pthread_barrier_init(&barrier_move_generation, NULL, N);
    pthread_barrier_init(&barrier_played_move_nq, NULL, N);
    pthread_barrier_init(&barrier_ID_0_bookeeping, NULL, N);
    pthread_barrier_init(&barrier_new_turn, NULL, N);
    pthread_barrier_init(&barrier_piece_lock, NULL, N);
    pthread_mutex_init(&game_over_lock, NULL);
}

void    create_N_threads(void){
    int i;
    for(i=0;i<N;i++){
        pthread_create(&(threadIDs[i]), NULL, &execute_thread_functs, NULL);
    }
}

void    join_all_threads(void){
    int i;
    for(i=0; i<N; i++){
        printf("Joining %d\n", i);
        pthread_join(threadIDs[i], NULL);
    }
}

void    init(void){
    rounds_best_move           = malloc(sizeof(MNODE));
    rounds_best_move->strength = 0;
    rounds_best_move->lock     = 0;
    game_over                  = 0;
    first_best_move_flag       = 0;
    total_move_count           = 0;
    season                     = WHITE_TO_MOVE;
    w_arsenal                  = NULL;
    b_arsenal                  = NULL;
    played_moves_head          = NULL;
    player_colour              = -1;
    game_state                 = STATE_COLOUR_CHOICE;
    white_king_has_moved       = 0;
    black_king_has_moved       = 0;
    white_left_rook_has_moved  = 0;
    black_left_rook_has_moved  = 0;
    white_right_rook_has_moved = 0;
    black_right_rook_has_moved = 0;
    srand( (unsigned)time(NULL) );

    init_locks();
    init_board();
}

void    sync(void){
    sleep(1);
    al_flip_display();
}

void    print_played_moves(void){
    MNODE *p = played_moves_head;
    int count = 1;
    while( p!=NULL ){
        printf("%d.(%d,%d)->(%d,%d)\n",count,p->old_c[0],p->old_c[1],p->new_c[0],p->new_c[1]);
        count++;
        p=p->next;
    }
}

void    show_game(void){
    v_init();
    int i;
    MNODE *p = played_moves_head;

    while( p != NULL ){
        v_print_board();
        v_print_pieces();
        update_board(p, 1);
        sync();
        p = p->next;
    }
    v_print_board();
    v_print_pieces();
    sync();
    getchar();
}