#include <stdlib.h>
#include <stdio.h>
#include "moves.h"
#include "lists.h"
#include "objects.h"
#include "pinning_mechanism.h"


int white_king_has_moved;
int black_king_has_moved;
int white_left_rook_has_moved;
int black_left_rook_has_moved;
int white_right_rook_has_moved;
int black_right_rook_has_moved;

void    m_create_pawn_moves(ANODE *p){
    int i = p->coords[0];
    int j = p->coords[1];
    int pin_direction = get_pin_direction(p);


    if( pin_direction!=RIGHT && pin_direction!=LEFT ){  // If pawn is pinned horizontaly, it cannot move at all!
        if( p->piece < 0 ){     // Black pawn
            if( j == 1){        // If first move
                if( (board[i][j+2]->piece==0) && (board[i][j+1]->piece==0) ){
                    if( pin_direction!=UPPER_RIGHT && pin_direction!=UPPER_LEFT && pin_direction!=LOWER_RIGHT && pin_direction!=LOWER_LEFT ){
                        MQ_insert(p, i, j+2);
                    }
                }
            }
            if( j < 7 ){
                if( board[i][j+1]->piece == 0 ){
                    if( pin_direction!=UPPER_RIGHT && pin_direction!=UPPER_LEFT && pin_direction!=LOWER_RIGHT && pin_direction!=LOWER_LEFT ){
                        MQ_insert(p, i, j+1);
                    }
                }
            }
            if ( (i > 0) && (j < 7) ){
                if( board[i-1][j+1]->piece > 0){
                    if( pin_direction<0 || pin_direction==LOWER_LEFT ){
                        MQ_insert(p, i-1, j+1);
                    }
                }
            }
            if ( (i < 7) && (j < 7 ) ){
                if( board[i+1][j+1]->piece > 0){
                    if( pin_direction<0 || pin_direction==LOWER_RIGHT ){
                        MQ_insert(p, i+1, j+1);
                    }
                }
            }
        }else{                  // White pawn
            if( j == 6){        // If first move
                if( board[i][j-2]->piece == 0 && (board[i][j-1]->piece==0)){
                    if( pin_direction!=UPPER_RIGHT && pin_direction!=UPPER_LEFT && pin_direction!=LOWER_RIGHT && pin_direction!=LOWER_LEFT ){
                        MQ_insert(p, i, j-2);
                    }
                }
            }
            if( j > 0 ){
                if( board[i][j-1]->piece == 0 ){
                    if( pin_direction!=UPPER_RIGHT && pin_direction!=UPPER_LEFT && pin_direction!=LOWER_RIGHT && pin_direction!=LOWER_LEFT ){
                        MQ_insert(p, i, j-1);
                    }
                }
            }
            if ( (i > 0) && (j > 0) ){
                if( board[i-1][j-1]->piece < 0){
                    if( pin_direction<0 || pin_direction==UPPER_LEFT ){
                        MQ_insert(p, i-1, j-1);
                    }
                }
            }
            if ( (i < 7) && (j > 0) ){
                if( board[i+1][j-1]->piece < 0)
                    if( pin_direction<0 || pin_direction==UPPER_RIGHT ){
                        MQ_insert(p, i+1, j-1);
                    }
            }
        }
    }
    
}

void    m_create_knight_moves(ANODE *p){
    int i = p->coords[0];
    int j = p->coords[1];

    if( !IsPinned(p) ){
        if( j != 0 ){
            if( i<6 ){
                if( ((board[i+2][j-1]->piece >=0) && (p->piece < 0)) ||    // Black moves
                    ((board[i+2][j-1]->piece <=0) && (p->piece > 0))    )  // White moves
                    MQ_insert(p, i+2, j-1);
            }    
            if( i>1 ){
                if( ((board[i-2][j-1]->piece >=0) && (p->piece < 0)) ||
                    ((board[i-2][j-1]->piece <=0) && (p->piece > 0))    )
                    MQ_insert(p, i-2, j-1);
            }     
        }
        if( j > 1){
            if( i<7 ){
                if( ((board[i+1][j-2]->piece >=0) && (p->piece < 0)) ||
                    ((board[i+1][j-2]->piece <=0) && (p->piece > 0))    )
                    MQ_insert(p, i+1, j-2);
            }
            if( i>0 ){
                if( ((board[i-1][j-2]->piece >=0) && (p->piece < 0)) ||
                    ((board[i-1][j-2]->piece <=0) && (p->piece > 0))    )
                    MQ_insert(p, i-1, j-2);
            }  
        }
        if( j<7 ){
            if( i>1 ){
                if( ((board[i-2][j+1]->piece >=0) && (p->piece < 0)) ||
                    ((board[i-2][j+1]->piece <=0) && (p->piece > 0))    )
                    MQ_insert(p, i-2, j+1);
            }
            if( i<6 ){
                if( ((board[i+2][j+1]->piece >=0) && (p->piece < 0)) ||
                    ((board[i+2][j+1]->piece <=0) && (p->piece > 0))    )
                    MQ_insert(p, i+2, j+1);
            }    
        }
        if( j<6 ){
            if( i<7 ){
                if( ((board[i+1][j+2]->piece >=0) && (p->piece < 0)) ||
                    ((board[i+1][j+2]->piece <=0) && (p->piece > 0))    )
                    MQ_insert(p, i+1, j+2);
            }
            if( i>0 ){
                if( ((board[i-1][j+2]->piece >=0) && (p->piece < 0)) ||
                    ((board[i-1][j+2]->piece <=0) && (p->piece > 0))    )
                    MQ_insert(p, i-1, j+2);
            }
        }
    }
}

void    m_create_bishop_moves(ANODE *p){
    int i = p->coords[0];
    int j = p->coords[1];
    int x = i+1; int y = j-1;
    int pin_direction = get_pin_direction(p);

    if( pin_direction!=LEFT && pin_direction!=RIGHT && pin_direction!=UP && pin_direction!=DOWN ){
        // Upper-Right
        while( (x<8) && (y>=0) ){
            if( ((board[x][y]->piece > 0) && (p->piece > 0)) ||     // Blocked by same
                ((board[x][y]->piece < 0) && (p->piece < 0))    )   // coloured piece
                break;
            if( board[x][y]->piece == 0 && (pin_direction<0 || pin_direction==UPPER_RIGHT) )
                MQ_insert(p, x, y);
            if( (((board[x][y]->piece >0) && (p->piece < 0)) || ((board[x][y]->piece <0) && (p->piece > 0))) &&
                  (pin_direction<0 || pin_direction==UPPER_RIGHT)                                                 ){
                MQ_insert(p, x, y);
                break;
            }
            x++; y--;
        }
        // Upper-Left
        x = i-1; y = j-1;
        while( (x>=0) && (y>=0) ){
            if( ((board[x][y]->piece > 0) && (p->piece > 0)) ||     // Blocked by same
                ((board[x][y]->piece < 0) && (p->piece < 0))    )   // coloured piece
                break;
            if( board[x][y]->piece == 0 && (pin_direction<0 || pin_direction==UPPER_LEFT) )
                MQ_insert(p, x, y);
            if( (((board[x][y]->piece >0) && (p->piece < 0)) || ((board[x][y]->piece <0) && (p->piece > 0))) &&
                  (pin_direction<0 || pin_direction==UPPER_LEFT)                                                  ){
                MQ_insert(p, x, y);
                break;
            }
            x--; y--;
        }
        // Lower-Right
        x = i+1; y = j+1;
        while( (x<8) && (y<8) ){
            if( ((board[x][y]->piece > 0) && (p->piece > 0)) ||     // Blocked by same
                ((board[x][y]->piece < 0) && (p->piece < 0))    )   // coloured piece
                break;
            if( board[x][y]->piece == 0 && (pin_direction<0 || pin_direction==LOWER_RIGHT) )
                MQ_insert(p, x, y);
            if( (((board[x][y]->piece >0) && (p->piece < 0)) || ((board[x][y]->piece <0) && (p->piece > 0))) && 
                  (pin_direction<0 || pin_direction==LOWER_RIGHT)                                                 ){
                MQ_insert(p, x, y);
                break;
            }
            x++; y++;
        }
        // Lower-Left
        x = i-1; y = j+1;
        while( (x>=0) && (y<8) ){
            if( ((board[x][y]->piece > 0) && (p->piece > 0)) ||     // Blocked by same
                ((board[x][y]->piece < 0) && (p->piece < 0))    )   // coloured piece
                break;
            if( board[x][y]->piece == 0 && (pin_direction<0 || pin_direction==LOWER_LEFT) )
                MQ_insert(p, x, y);
            if( (((board[x][y]->piece >0) && (p->piece < 0)) || ((board[x][y]->piece <0) && (p->piece > 0))) &&
                  (pin_direction<0 || pin_direction==LOWER_LEFT)                                                 ){
                MQ_insert(p, x, y);
                break;
            }
            x--; y++;
        }
    }
}

void    m_create_rook_moves(ANODE *p){
    int i = p->coords[0];
    int j = p->coords[1];
    int x = i+1; int y = j;
    int pin_direction = get_pin_direction(p);

    if( pin_direction!=UPPER_RIGHT && pin_direction!=UPPER_LEFT && pin_direction!=LOWER_RIGHT && pin_direction!=LOWER_LEFT ){
        // Right
        while( x<8 ){
            if( ((board[x][y]->piece > 0) && (p->piece > 0)) ||     // Blocked by same
                ((board[x][y]->piece < 0) && (p->piece < 0))    )   // coloured piece
                break;
            if( board[x][y]->piece == 0 && (pin_direction<0 || pin_direction==RIGHT) )
                MQ_insert(p, x, y);
            if( (((board[x][y]->piece >0) && (p->piece < 0)) || ((board[x][y]->piece <0) && (p->piece > 0))) &&
                  (pin_direction<0 || pin_direction==RIGHT)                                               ){
                MQ_insert(p, x, y);
                break;
            }
            x++;
        }
        // Left
        x = i-1;
        while( x>=0 ){
            if( ((board[x][y]->piece > 0) && (p->piece > 0)) ||     // Blocked by same
                ((board[x][y]->piece < 0) && (p->piece < 0))    )   // coloured piece
                break;
            if( board[x][y]->piece == 0 && (pin_direction<0 || pin_direction==LEFT) )
                MQ_insert(p, x, y);
            if( (((board[x][y]->piece >0) && (p->piece < 0)) || ((board[x][y]->piece <0) && (p->piece > 0))) &&
                  (pin_direction<0 || pin_direction==LEFT)                                                     ){
                MQ_insert(p, x, y);
                break;
            }
            x--;
        }
        // Down
        x = i; y = j+1;
        while( y<8 ){
            if( ((board[x][y]->piece > 0) && (p->piece > 0)) ||     // Blocked by same
                ((board[x][y]->piece < 0) && (p->piece < 0))    )   // coloured piece
                break;
            if( board[x][y]->piece == 0 && (pin_direction<0 || pin_direction==DOWN) )
                MQ_insert(p, x, y);
            if( (((board[x][y]->piece >0) && (p->piece < 0)) || ((board[x][y]->piece <0) && (p->piece > 0))) &&
                  (pin_direction<0 || pin_direction==DOWN)                                                      ){
                MQ_insert(p, x, y);
                break;
            }
            y++;
        }
        // Up
        y = j-1;
        while( y>=0 ){
            if( ((board[x][y]->piece > 0) && (p->piece > 0)) ||     // Blocked by same
                ((board[x][y]->piece < 0) && (p->piece < 0))    )   // coloured piece
                break;
            if( board[x][y]->piece == 0 && (pin_direction<0 || pin_direction==UP) )
                MQ_insert(p, x, y);
            if( (((board[x][y]->piece >0) && (p->piece < 0)) || ((board[x][y]->piece <0) && (p->piece > 0))) &&
                  (pin_direction<0 || pin_direction==UP)                                                        ){
                MQ_insert(p, x, y);
                break;
            }
            y--;
        }
    }
}

void    m_create_queen_moves(ANODE *p){
    m_create_bishop_moves(p);
    m_create_rook_moves(p);
}

void    m_create_king_moves(ANODE *p){
    int i = p->coords[0];
    int j = p->coords[1];
    int dummy_x, dummy_y;
    int square_covered_by_enemy;
    int try_short_castle;
    int try_long_castle;
    int king_has_moved       = season==WHITE_TO_MOVE ? white_king_has_moved       : black_king_has_moved;
    int left_rook_has_moved  = season==WHITE_TO_MOVE ? white_left_rook_has_moved  : black_left_rook_has_moved;
    int right_rook_has_moved = season==WHITE_TO_MOVE ? white_right_rook_has_moved : black_right_rook_has_moved;
    int attacker = (season==WHITE_TO_MOVE) ? BLACK_PIECE : WHITE_PIECE;
    MNODE *dummy_move = malloc(sizeof(MNODE));
    dummy_move->old_c[0] = i;
    dummy_move->old_c[1] = j;
    dummy_move->new_c[0] = i+1;
    dummy_move->new_c[1] = j;


    if( (defend_from_check_flag==1) || (king_has_moved==1) ){
        try_short_castle = 0;
        try_long_castle  = 0;
    }
    else{
        try_short_castle = ((board[i+1][j]->piece==0) & (board[i+2][j]->piece==0)) & (right_rook_has_moved==0);
        try_long_castle  = ((board[i-1][j]->piece==0) & (board[i-2][j]->piece==0) & (board[i-3][j]->piece==0)) & ( left_rook_has_moved==0);
    }

    printf("========================================== Try Short = %d, Try long = %d\n", try_short_castle, try_long_castle);
// Castling
    if( try_short_castle ){
    // Check if F1 or F8 is covered by enemy piece;
        square_covered_by_enemy = m_look_for_protectors(attacker, dummy_move, &dummy_x, &dummy_y);
        if( square_covered_by_enemy==0 ){
        // Check if G1 or G8 is covered by enemy piece;
            dummy_move->new_c[0] = i+2;
            square_covered_by_enemy = m_look_for_protectors(attacker, dummy_move, &dummy_x, &dummy_y);
            if( square_covered_by_enemy==0 ){
                MQ_insert(p, i+2, j);
            }
        }
    }
    if( try_long_castle ){
    // Check if D1 or D8 is covered by enemy piece;
        dummy_move->new_c[0] = i-1;
        square_covered_by_enemy = m_look_for_protectors(attacker, dummy_move, &dummy_x, &dummy_y);
        if( square_covered_by_enemy==0 ){
        // Check if C1 or C8 is covered by enemy piece;
            dummy_move->new_c[0] = i-2;
            square_covered_by_enemy = m_look_for_protectors(attacker, dummy_move, &dummy_x, &dummy_y);
            if( square_covered_by_enemy==0 ){
                MQ_insert(p, i-2, j);
            }
        }
    }
    if( j<7 ){
        if( (board[i][j+1]->piece >= 0) && (p->piece < 0) ||
            (board[i][j+1]->piece <= 0) && (p->piece > 0)    )
            MQ_insert(p, i, j+1);
        if( i>0 ){
            if( (board[i-1][j+1]->piece >= 0) && (p->piece < 0) ||
                (board[i-1][j+1]->piece <= 0) && (p->piece > 0)    )
                MQ_insert(p, i-1, j+1);
        }
        if( i<7 ){
            if( (board[i+1][j+1]->piece >= 0) && (p->piece < 0) ||
                (board[i+1][j+1]->piece <= 0) && (p->piece > 0)    )
                MQ_insert(p, i+1, j+1);
        }
    }
    if( j>0 ){
        if( (board[i][j-1]->piece >= 0) && (p->piece < 0) ||
            (board[i][j-1]->piece <= 0) && (p->piece > 0)    )
            MQ_insert(p, i, j-1);
        if( i>0 ){
            if( (board[i-1][j-1]->piece >= 0) && (p->piece < 0) ||
                (board[i-1][j-1]->piece <= 0) && (p->piece > 0)    )
                MQ_insert(p, i-1, j-1);
        }
        if( i<7 ){
            if( (board[i+1][j-1]->piece >= 0) && (p->piece < 0) ||
                (board[i+1][j-1]->piece <= 0) && (p->piece > 0)    )
                MQ_insert(p, i+1, j-1);
        }
    }
    if( i>0 ){
        if( (board[i-1][j]->piece >= 0) && (p->piece < 0) ||
            (board[i-1][j]->piece <= 0) && (p->piece > 0)    )
            MQ_insert(p, i-1, j);
    }
    if( i<7 ){
        if( (board[i+1][j]->piece >= 0) && (p->piece < 0) ||
            (board[i+1][j]->piece <= 0) && (p->piece > 0)    )
            MQ_insert(p, i+1, j);
    }
}

// Not used in project, changed it with function below
void    m_delete_move_queue(ANODE *p){
    MNODE *prev = NULL;
    MNODE *curr = p->mhead;

    while( curr != NULL ){
        prev = curr;
        curr = curr->next;
        free(prev);
    }

    p->mhead = NULL;
}
//

int     m_move_dequeue(ANODE *Q){
    MNODE *first, *last, *next;

    while( 1 ){
        first = Q->mhead;
        last  = Q->mtail;
        next  = first->next;
        if( first == Q->mhead ){
            if( first == last ){
                if( next == NULL )
                    return 0;
                __sync_bool_compare_and_swap(&(Q->mtail), last, next, NULL);
            }
            else{
                if( __sync_bool_compare_and_swap(&(Q->mhead), first, next, NULL) )
                    break;
            }
        }
    }
    return 1;
}

void    m_print_all_moves(void){
    ANODE *p;   MNODE *ptr;
    if( season == WHITE_TO_MOVE )
        p = w_arsenal;
    else
        p = b_arsenal;

    while( p!=NULL ){
        ptr = p->mhead;
        printf("\t%d (%d,%d) MOVES: ", p->piece, p->coords[0], p->coords[1]);
        while(ptr != NULL){
            printf("(%d, %d [%d]), ", ptr->new_c[0], ptr->new_c[1], ptr->strength);
            ptr = ptr->next;
        }
        printf("\n");
        p = p->next;
    }
}

int     m_check_diagonals_for_protection(int colour, MNODE *move, int *attx, int *atty){
    int result = 0;
    int self_x = move->old_c[0];
    int self_y = move->old_c[1];
    int i = move->new_c[0];
    int j = move->new_c[1];
    int x = i+1; int y = j-1;

// UPPER RIGHT DIAGONAL
    // First check if square is covered by pawn or king
    // This square (x = i+1 and y = j-1) can only be protected by a White king, not a white pawn
    if( (x<8) && (y>=0) ){
        if( (colour==WHITE_PIECE) && (board[x][y]->piece == KING) ){
            result+= KING;
        }
        else if( (colour==BLACK_PIECE) && ((board[x][y]->piece == -PAWN) || (board[x][y]->piece == -KING)) ){
            result += board[x][y]->piece;
            *attx = x;
            *atty = y;
        }
        else{
            // Square not protected by either pawn or king.
        }
    }
    // Then check the whole diagonal for bishop/queen protection
    while( (x<8) && (y>=0) ){
        if( board[x][y]->piece != 0 && !(x==self_x && y==self_y) ){
            if( colour==WHITE_PIECE ){
                if( (board[x][y]->piece==BISHOP) || (board[x][y]->piece==QUEEN) ){
                    result += board[x][y]->piece;
                    *attx = x;
                    *atty = y;
                }
            }
            else{
                if( (board[x][y]->piece==-BISHOP) || (board[x][y]->piece==-QUEEN) ){
                    result += board[x][y]->piece;
                    *attx = x;
                    *atty = y;
                }
            }
            break;
        }
        x++; y--;
    }
    
// UPPER LEFT DIAGONAL
    x = i-1; y = j-1;
    // Check for pawn on white's turn
    if( (x>=0) && (y>=0) ){
        if( (colour==WHITE_PIECE) && (board[x][y]->piece == KING) ){
            result+= KING;
        }
        else if( (colour==BLACK_PIECE) && ((board[x][y]->piece == -PAWN) || (board[x][y]->piece == -KING)) ){
            result += board[x][y]->piece;
            *attx = x;
            *atty = y;
        }
        else{
            // Square not protected by either pawn or king.
        }
    }
    // Then check the whole diagonal for bishop/queen protection
    while( (x>=0) && (y>=0) ){
        if( board[x][y]->piece != 0 && !(x==self_x && y==self_y) ){
            if( colour==WHITE_PIECE ){
                if( (board[x][y]->piece==BISHOP) || (board[x][y]->piece==QUEEN) ){
                    result += board[x][y]->piece;
                    *attx = x;
                    *atty = y;
                }
            }
            else{
                if( (board[x][y]->piece==-BISHOP) || (board[x][y]->piece==-QUEEN) ){
                    result += board[x][y]->piece;
                    *attx = x;
                    *atty = y;
                }
            }
            break;
        }
        x--; y--;
    }

// LOWER RIGHT DIAGONAL
    x = i+1; y = j+1;
    // Check for pawn on black's turn
    if( (x<8) && (y<8) ){
        if( (colour==BLACK_PIECE) && (board[x][y]->piece==-KING) ){
            result+= -KING;
        }
        else if( (colour==WHITE_PIECE) && ((board[x][y]->piece == PAWN) || (board[x][y]->piece == KING)) ){
            result += board[x][y]->piece;
            *attx = x;
            *atty = y;
        }
        else{
            // Square not protected by either pawn or king.
        }
    }
    while( (x<8) && (y<8) ){
        if( board[x][y]->piece != 0 && !(x==self_x && y==self_y) ){
            if( colour==WHITE_PIECE ){
                if( (board[x][y]->piece==BISHOP) || (board[x][y]->piece==QUEEN) ){
                    result += board[x][y]->piece;
                    *attx = x;
                    *atty = y;
                }
            }
            else{
                if( (board[x][y]->piece==-BISHOP) || (board[x][y]->piece==-QUEEN) ){
                    result += board[x][y]->piece;
                    *attx = x;
                    *atty = y;
                }
            }
            break;
        }
        x++; y++;
    }

// LOWER LEFT DIAGONAL
    x = i-1; y = j+1;
    // Check for pawn on black's turn
    if( (x>=0) && (y<8) ){
        if( (colour==BLACK_PIECE) && (board[x][y]->piece==-KING) ){
            result+= -KING;
        }
        else if( (colour==WHITE_PIECE) && ((board[x][y]->piece == PAWN) || (board[x][y]->piece == KING)) ){
            result += board[x][y]->piece;
            *attx = x;
            *atty = y;
        }
        else{
            // Square not protected by either pawn or king.
        }
    }
    while( (x>=0) && (y<8) ){
        if( board[x][y]->piece != 0 && !(x==self_x && y==self_y) ){
            if( colour==WHITE_PIECE ){
                if( (board[x][y]->piece==BISHOP) || (board[x][y]->piece==QUEEN) ){
                    result += board[x][y]->piece;
                    *attx = x;
                    *atty = y;
                }
            }
            else{
                if( (board[x][y]->piece==-BISHOP) || (board[x][y]->piece==-QUEEN) ){
                    result += board[x][y]->piece;
                    *attx = x;
                    *atty = y;
                }
            }
            break;
        }
        x--; y++;
    }

    return result;
}

int     m_check_rows_and_columns_for_protection(int colour, MNODE *move, int *attx, int *atty){
    int result = 0;
    int self_x = move->old_c[0];
    int self_y = move->old_c[1];
    int i = move->new_c[0];
    int j = move->new_c[1];
    int x = i; int y = j+1;

// UP
    if( y<8 ){
        if( ((colour==WHITE_PIECE) && (board[x][y]->piece == KING)) ||
            ((colour==BLACK_PIECE) && (board[x][y]->piece == -KING))   ){
            result += board[x][y]->piece;
            *attx = x;
            *atty = y;
        }
        else{
            while( y<8 ){
                if( board[x][y]->piece != 0 && !(x==self_x && y==self_y) ){
                    if( colour==WHITE_PIECE ){
                        if( (board[x][y]->piece==ROOK) || (board[x][y]->piece==QUEEN) ){
                            result += board[x][y]->piece;
                            *attx = x;
                            *atty = y;
                        }
                    }
                    else{
                        if( (board[x][y]->piece==-ROOK) || (board[x][y]->piece==-QUEEN) ){
                            result += board[x][y]->piece;
                            *attx = x;
                            *atty = y;
                        }
                    }
                    break;
                }
                y++;
            }
        }
    }
// DOWN
    y = j-1;
    if( y>=0 ){
        if( ((colour==WHITE_PIECE) && (board[x][y]->piece == KING)) ||
            ((colour==BLACK_PIECE) && (board[x][y]->piece == -KING))   ){
            result += board[x][y]->piece;
            *attx = x;
            *atty = y;
        }
        else{
            while( y>=0 ){
                if( board[x][y]->piece != 0 && !(x==self_x && y==self_y) ){
                    if( colour==WHITE_PIECE ){
                        if( (board[x][y]->piece==ROOK) || (board[x][y]->piece==QUEEN) ){
                            result += board[x][y]->piece;
                            *attx = x;
                            *atty = y;
                        }
                    }
                    else{
                        if( (board[x][y]->piece==-ROOK) || (board[x][y]->piece==-QUEEN) ){
                            result += board[x][y]->piece;
                            *attx = x;
                            *atty = y;
                        }
                    }
                    break;
                }
                y--;
            }
        }
    }
// RIGHT
    x = i+1; y = j;
    if( x<8 ){
        if( ((colour==WHITE_PIECE) && (board[x][y]->piece == KING)) ||
            ((colour==BLACK_PIECE) && (board[x][y]->piece == -KING))      ){
            result += board[x][y]->piece;
            *attx = x;
            *atty = y;
        }
        else{
            while( x<8 ){
                if( board[x][y]->piece != 0 && !(x==self_x && y==self_y) ){
                    if( colour==WHITE_PIECE ){
                        if( (board[x][y]->piece==ROOK) || (board[x][y]->piece==QUEEN) ){
                            result += board[x][y]->piece;
                            *attx = x;
                            *atty = y;
                        }
                    }
                    else{
                        if( (board[x][y]->piece==-ROOK) || (board[x][y]->piece==-QUEEN) ){
                            result += board[x][y]->piece;
                            *attx = x;
                            *atty = y;
                        }
                    }
                    break;
                }
                x++;
            }
        }
    }
// LEFT
    x = i-1;
    if( x>=0 ){
        if( ((colour==WHITE_PIECE) && (board[x][y]->piece == KING)) ||
            ((colour==BLACK_PIECE) && (board[x][y]->piece == -KING))      ){
            result += board[x][y]->piece;
            *attx = x;
            *atty = y;
        }
        else{
            while( x>=0 ){
                if( board[x][y]->piece != 0 && !(x==self_x && y==self_y) ){
                    if( colour==WHITE_PIECE ){
                        if( (board[x][y]->piece==ROOK) || (board[x][y]->piece==QUEEN) ){
                            result += board[x][y]->piece;
                            *attx = x;
                            *atty = y;
                        }
                    }
                    else{
                        if( (board[x][y]->piece==-ROOK) || (board[x][y]->piece==-QUEEN) ){
                            result += board[x][y]->piece;
                            *attx = x;
                            *atty = y;
                        }
                    }
                    break;
                }
                x--;
            }
        }
    }

    return result;
}

int     m_check_for_protection_by_knight(int colour, MNODE *move, int *attx, int *atty){
    int result=0;
    int i = move->new_c[0];
    int j = move->new_c[1];

    if( j != 0 ){
        if( i<6 ){
            if( ((board[i+2][j-1]->piece==KNIGHT) && (colour==WHITE_PIECE)) ||
                ((board[i+2][j-1]->piece==-KNIGHT) && (colour==BLACK_PIECE))    ){
                result += board[i+2][j-1]->piece;
                *attx = i+2;
                *atty = j-1;
            }
        }    
        if( i>1 ){
            if( ((board[i-2][j-1]->piece==KNIGHT) && (colour==WHITE_PIECE)) ||
                ((board[i-2][j-1]->piece==-KNIGHT) && (colour==BLACK_PIECE))    ){
                result += board[i-2][j-1]->piece;
                *attx = i-2;
                *atty = j-1;
            }
        }     
    }
    if( j > 1){
        if( i<7 ){
            if( ((board[i+1][j-2]->piece==KNIGHT) && (colour==WHITE_PIECE)) ||
                ((board[i+1][j-2]->piece==-KNIGHT) && (colour==BLACK_PIECE))    ){
                result += board[i+1][j-2]->piece;
                *attx = i+1;
                *atty = j-2;
            }
        }
        if( i>0 ){
            if( ((board[i-1][j-2]->piece==KNIGHT) && (colour==WHITE_PIECE)) ||
                ((board[i-1][j-2]->piece==-KNIGHT) && (colour==BLACK_PIECE))    ){
                result += board[i-1][j-2]->piece;
                *attx = i-1;
                *atty = j-2;
            }
        }  
    }
    if( j<7 ){
        if( i>1 ){
            if( ((board[i-2][j+1]->piece==KNIGHT) && (colour==WHITE_PIECE)) ||
                ((board[i-2][j+1]->piece==-KNIGHT) && (colour==BLACK_PIECE))    ){
                result += board[i-2][j+1]->piece;
                *attx = i-2;
                *atty = j+1;
            }
        }
        if( i<6 ){
            if( ((board[i+2][j+1]->piece==KNIGHT) && (colour==WHITE_PIECE)) ||
                ((board[i+2][j+1]->piece==-KNIGHT) && (colour==BLACK_PIECE))    ){
                result += board[i+2][j+1]->piece;
                *attx = i+2;
                *atty = j+1;
            }
        }    
    }
    if( j<6 ){
        if( i<7 ){
            if( ((board[i+1][j+2]->piece==KNIGHT) && (colour==WHITE_PIECE)) ||
                ((board[i+1][j+2]->piece==-KNIGHT) && (colour==BLACK_PIECE))    ){
                result += board[i+1][j+2]->piece;
                *attx = i+1;
                *atty = j+2;
            }
        }
        if( i>0 ){
            if( ((board[i-1][j+2]->piece==KNIGHT) && (colour==WHITE_PIECE)) ||
                ((board[i-1][j+2]->piece==-KNIGHT) && (colour==BLACK_PIECE))    ){
                result += board[i-1][j+2]->piece;
                *attx = i-1;
                *atty = j+2;
            }
        }
    }
    return result;
}

int     exclude_curr_piece_from_protectors(int *a, int *b, int *c){
    int i;
    int x[3] = {*a, *b, *c};

// a, b, c are always going to be != 0 since the piece-about-to-move is always covering the square
// it is going to land on.
    for( i=0; i<3; i++ ){
        if( x[i]==PAWN   || x[i]==-PAWN   || x[i]==KNIGHT || x[i]==-KNIGHT ||
            x[i]==BISHOP || x[i]==-BISHOP || x[i]==ROOK   || x[i]==-ROOK   ||
            x[i]==QUEEN  || x[i]==-QUEEN  || x[i]==KING   || x[i]==-KING      ){
                x[i] = 0;
            }
    }
    *a = x[0];
    *b = x[1];
    *c = x[2];
}

int     m_look_for_protectors(int colour, MNODE *move, int *x, int *y){
    int result, a,b,c;
    int i = move->new_c[0];
    int j = move->new_c[1];
    int attacker_x = -1;
    int attacker_y = -1;

    a = m_check_diagonals_for_protection(colour,move, &attacker_x, &attacker_y);
    b = m_check_for_protection_by_knight(colour,move, &attacker_x, &attacker_y);
    c = m_check_rows_and_columns_for_protection(colour,move, &attacker_x, &attacker_y);

    if( (colour==WHITE_PIECE && season==WHITE_TO_MOVE) ||
        (colour==BLACK_PIECE && season==BLACK_TO_MOVE)    ){
        exclude_curr_piece_from_protectors(&a, &b, &c);
        printf("DEFENDERS at (%d,%d): %d,%d,%d\n", i,j,a,b,c);
    }
    else{
        printf("ATTACKERS at (%d,%d): %d,%d,%d\n", i,j,a,b,c);
    }

    result = a+b+c;
    *x = attacker_x;
    *y = attacker_y;

    return result; 
}