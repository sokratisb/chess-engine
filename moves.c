#include <stdlib.h>
#include <stdio.h>
#include "moves.h"
#include "lists.h"
#include "objects.h"
#include "pinning_mechanism.h"


void    m_create_pawn_moves(ANODE *p){
    int i = p->coords[0];
    int j = p->coords[1];
    
    if( !IsPinned(p) ){
        if( p->piece < 0 ){     // Black pawn
            if( j == 1){        // If first move
                if( (board[i][j+2]->piece==0) && (board[i][j+1]->piece==0) )
                    MQ_insert(p, i, j+2);
            }
            if( j < 7 ){
                if( board[i][j+1]->piece == 0 )
                    MQ_insert(p, i, j+1);
            }
            if ( (i > 0) && (j < 7) ){
                if( board[i-1][j+1]->piece > 0)
                    MQ_insert(p, i-1, j+1);
            }
            if ( (i < 7) && (j < 7 ) ){
                if( board[i+1][j+1]->piece > 0)
                    MQ_insert(p, i+1, j+1);
            }
        }else{                  // White pawn
            if( j == 6){        // If first move
                if( board[i][j-2]->piece == 0 && (board[i][j-1]->piece==0))
                    MQ_insert(p, i, j-2);
            }
            if( j > 0 ){
                if( board[i][j-1]->piece == 0 )
                    MQ_insert(p, i, j-1);
            }
            if ( (i > 0) && (j > 0) ){
                if( board[i-1][j-1]->piece < 0)
                    MQ_insert(p, i-1, j-1);
            }
            if ( (i < 7) && (j > 0) ){
                if( board[i+1][j-1]->piece < 0)
                    MQ_insert(p, i+1, j-1);
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

    if( !IsPinned(p) ){
        // Upper-Right
        while( (x<8) && (y>=0) ){
            if( ((board[x][y]->piece > 0) && (p->piece > 0)) ||     // Blocked by same
                ((board[x][y]->piece < 0) && (p->piece < 0))    )   // coloured piece
                break;
            if( board[x][y]->piece == 0 )
                MQ_insert(p, x, y);
            if( ((board[x][y]->piece >0) && (p->piece < 0)) ||
                ((board[x][y]->piece <0) && (p->piece > 0))    ){
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
            if( board[x][y]->piece == 0 )
                MQ_insert(p, x, y);
            if( ((board[x][y]->piece >0) && (p->piece < 0)) ||
                ((board[x][y]->piece <0) && (p->piece > 0))    ){
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
            if( board[x][y]->piece == 0 )
                MQ_insert(p, x, y);
            if( ((board[x][y]->piece >0) && (p->piece < 0)) ||
                ((board[x][y]->piece <0) && (p->piece > 0))    ){
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
            if( board[x][y]->piece == 0 )
                MQ_insert(p, x, y);
            if( ((board[x][y]->piece >0) && (p->piece < 0)) ||
                ((board[x][y]->piece <0) && (p->piece > 0))    ){
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

    if( !IsPinned(p) ){
        // Right
        while( x<8 ){
            if( ((board[x][y]->piece > 0) && (p->piece > 0)) ||     // Blocked by same
                ((board[x][y]->piece < 0) && (p->piece < 0))    )   // coloured piece
                break;
            if( board[x][y]->piece == 0 )
                MQ_insert(p, x, y);
            if( ((board[x][y]->piece >0) && (p->piece < 0)) ||
                ((board[x][y]->piece <0) && (p->piece > 0))    ){
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
            if( board[x][y]->piece == 0 )
                MQ_insert(p, x, y);
            if( ((board[x][y]->piece >0) && (p->piece < 0)) ||
                ((board[x][y]->piece <0) && (p->piece > 0))    ){
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
            if( board[x][y]->piece == 0 )
                MQ_insert(p, x, y);
            if( ((board[x][y]->piece >0) && (p->piece < 0)) ||
                ((board[x][y]->piece <0) && (p->piece > 0))    ){
                MQ_insert(p, x, y);
                break;
            }
            y++;
        }
        // up
        y = j-1;
        while( y>=0 ){
            if( ((board[x][y]->piece > 0) && (p->piece > 0)) ||     // Blocked by same
                ((board[x][y]->piece < 0) && (p->piece < 0))    )   // coloured piece
                break;
            if( board[x][y]->piece == 0 )
                MQ_insert(p, x, y);
            if( ((board[x][y]->piece >0) && (p->piece < 0)) ||
                ((board[x][y]->piece <0) && (p->piece > 0))    ){
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

int     m_check_diagonals_for_attacker(int i, int j){
    int result = 0;
    int x = i+1; int y = j-1;

// UPPER RIGHT DIAGONAL
    // Check for pawn on white's turn
    if( (x<8) && (y>0) && (season==WHITE_TO_MOVE) &&
        ((board[x][y]->piece == -PAWN) || (board[x][y]->piece == -KING)) )
        result += board[x][y]->piece;
    else{
        while( (x<8) && (y>=0) ){
            if( board[x][y]->piece != 0 ){
                if( season == WHITE_TO_MOVE ){
                    if( (board[x][y]->piece==-BISHOP) || (board[x][y]->piece==-QUEEN) )
                        result += board[x][y]->piece;
                }
                else{
                    if( (board[x][y]->piece==BISHOP) || (board[x][y]->piece==QUEEN) )
                        result += board[x][y]->piece;
                }
                break;
            }
            x++; y--;
        }
    }
// UPPER LEFT DIAGONAL
    x = i-1; y = j-1;
    // Check for pawn on white's turn
    if( (x>=0) && (y>0) && (season==WHITE_TO_MOVE) && 
      ((board[x][y]->piece == -PAWN) || (board[x][y]->piece == -KING)) )
        result += board[x][y]->piece;
    else{
        while( (x>=0) && (y>=0) ){
            if( board[x][y]->piece != 0 ){
                if( season == WHITE_TO_MOVE ){
                    if( (board[x][y]->piece==-BISHOP) || (board[x][y]->piece==-QUEEN) )
                        result += board[x][y]->piece;
                }
                else{
                    if( (board[x][y]->piece==BISHOP) || (board[x][y]->piece==QUEEN) )
                        result += board[x][y]->piece;
                }
                break;
            }
            x--; y--;
        }
    }
// LOWER RIGHT DIAGONAL
    x = i+1; y = j+1;
    // Check for pawn on black's turn
    if( (x<8) && (y<8) && (season==BLACK_TO_MOVE) && 
      ((board[x][y]->piece == PAWN) || (board[x][y]->piece == KING)) )
        result += board[x][y]->piece;
    else{
        while( (x<8) && (y<8) ){
            if( board[x][y]->piece != 0 ){
                if( season == WHITE_TO_MOVE ){
                    if( (board[x][y]->piece==-BISHOP) || (board[x][y]->piece==-QUEEN) )
                        result += board[x][y]->piece;
                }
                else{
                    if( (board[x][y]->piece==BISHOP) || (board[x][y]->piece==QUEEN) )
                        result += board[x][y]->piece;
                }
                break;
            }
            x++; y++;
        }
    }
// LOWER LEFT DIAGONAL
    x = i-1; y = j+1;
    // Check for pawn on black's turn
    if( (x>=0) && (y<8) && (season==BLACK_TO_MOVE) && 
      ((board[x][y]->piece == PAWN) || (board[x][y]->piece == KING)) )
        result += board[x][y]->piece;
    else{
        while( (x>=0) && (y<8) ){
            if( board[x][y]->piece != 0 ){
                if( season == WHITE_TO_MOVE ){
                    if( (board[x][y]->piece==-BISHOP) || (board[x][y]->piece==-QUEEN) )
                        result += board[x][y]->piece;
                }
                else{
                    if( (board[x][y]->piece==BISHOP) || (board[x][y]->piece==QUEEN) )
                        result += board[x][y]->piece;
                }
                break;
            }
            x--; y++;
        }
    }

    return result;
}

int     m_check_rows_and_columns_for_attacker(int i, int j){
    int result = 0;
    int x = i; int y = j+1;

// UP
    if( y<8 ){
        if( ((season==WHITE_TO_MOVE) && (board[x][y]->piece == -KING)) ||
            ((season==BLACK_TO_MOVE) && (board[x][y]->piece == KING))      )
            result += board[x][y]->piece;
        else{
            while( y<8 ){
                if( board[x][y]->piece != 0 ){
                    if( season == WHITE_TO_MOVE ){
                        if( (board[x][y]->piece==-ROOK) || (board[x][y]->piece==-QUEEN) )
                            result += board[x][y]->piece;
                    }
                    else{
                        if( (board[x][y]->piece==ROOK) || (board[x][y]->piece==QUEEN) )
                            result += board[x][y]->piece;
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
        if( ((season==WHITE_TO_MOVE) && (board[x][y]->piece == -KING)) ||
            ((season==BLACK_TO_MOVE) && (board[x][y]->piece == KING))      )
            result += board[x][y]->piece;
        else{
            while( y>=0 ){
                if( board[x][y]->piece != 0 ){
                    if( season == WHITE_TO_MOVE ){
                        if( (board[x][y]->piece==-ROOK) || (board[x][y]->piece==-QUEEN) )
                            result += board[x][y]->piece;
                    }
                    else{
                        if( (board[x][y]->piece==ROOK) || (board[x][y]->piece==QUEEN) )
                            result += board[x][y]->piece;
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
        if( ((season==WHITE_TO_MOVE) && (board[x][y]->piece == -KING)) ||
            ((season==BLACK_TO_MOVE) && (board[x][y]->piece == KING))      )
            result += board[x][y]->piece;
        else{
            while( x<8 ){
                if( board[x][y]->piece != 0 ){
                    if( season == WHITE_TO_MOVE ){
                        if( (board[x][y]->piece==-ROOK) || (board[x][y]->piece==-QUEEN) )
                            result += board[x][y]->piece;
                    }
                    else{
                        if( (board[x][y]->piece==ROOK) || (board[x][y]->piece==QUEEN) )
                            result += board[x][y]->piece;
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
        if( ((season==WHITE_TO_MOVE) && (board[x][y]->piece == -KING)) ||
            ((season==BLACK_TO_MOVE) && (board[x][y]->piece == KING))      )
            result += board[x][y]->piece;
        else{
            while( x>=0 ){
                if( board[x][y]->piece != 0 ){
                    if( season == WHITE_TO_MOVE ){
                        if( (board[x][y]->piece==-ROOK) || (board[x][y]->piece==-QUEEN) )
                            result += board[x][y]->piece;
                    }
                    else{
                        if( (board[x][y]->piece==ROOK) || (board[x][y]->piece==QUEEN) )
                            result += board[x][y]->piece;
                    }
                    break;
                }
                x--;
            }
        }
    }

    return result;
}

int     m_check_for_attack_by_knight(int i, int j){
    int result=0;
    if( j != 0 ){
        if( i<6 ){
            if( ((board[i+2][j-1]->piece==-KNIGHT) && (season == WHITE_TO_MOVE)) ||
                ((board[i+2][j-1]->piece== KNIGHT) && (season == BLACK_TO_MOVE))    )
                result += board[i+2][j-1]->piece;
        }    
        if( i>1 ){
            if( ((board[i-2][j-1]->piece==-KNIGHT) && (season == WHITE_TO_MOVE)) ||
                ((board[i-2][j-1]->piece== KNIGHT) && (season == BLACK_TO_MOVE))    )
                result += board[i-2][j-1]->piece;
        }     
    }
    if( j > 1){
        if( i<7 ){
            if( ((board[i+1][j-2]->piece==-KNIGHT) && (season == WHITE_TO_MOVE)) ||
                ((board[i+1][j-2]->piece== KNIGHT) && (season == BLACK_TO_MOVE))    )
                result += board[i+1][j-2]->piece;
        }
        if( i>0 ){
            if( ((board[i-1][j-2]->piece==-KNIGHT) && (season == WHITE_TO_MOVE)) ||
                ((board[i-1][j-2]->piece== KNIGHT) && (season == BLACK_TO_MOVE))    )
                result += board[i-1][j-2]->piece;
        }  
    }
    if( j<7 ){
        if( i>1 ){
            if( ((board[i-2][j+1]->piece==-KNIGHT) && (season == WHITE_TO_MOVE)) ||
                ((board[i-2][j+1]->piece== KNIGHT) && (season == BLACK_TO_MOVE))    )
                result += board[i-2][j+1]->piece;
        }
        if( i<6 ){
            if( ((board[i+2][j+1]->piece==-KNIGHT) && (season == WHITE_TO_MOVE)) ||
                ((board[i+2][j+1]->piece== KNIGHT) && (season == BLACK_TO_MOVE))    )
                result += board[i+2][j+1]->piece;
        }    
    }
    if( j<6 ){
        if( i<7 ){
            if( ((board[i+1][j+2]->piece==-KNIGHT) && (season == WHITE_TO_MOVE)) ||
                ((board[i+1][j+2]->piece== KNIGHT) && (season == BLACK_TO_MOVE))    )
                result += board[i+1][j+2]->piece;
        }
        if( i>0 ){
            if( ((board[i-1][j+2]->piece==-KNIGHT) && (season == WHITE_TO_MOVE)) ||
                ((board[i-1][j+2]->piece== KNIGHT) && (season == BLACK_TO_MOVE))    )
                result += board[i-1][j+2]->piece;
        }
    }
    return result;
}

int     m_check_for_attackers(int i, int j){
    int result, a,b,c;

    a = m_check_diagonals_for_attacker(i,j);
    b = m_check_for_attack_by_knight(i,j);
    c = m_check_rows_and_columns_for_attacker(i,j);

    result = a+b+c;

    return result; 
}