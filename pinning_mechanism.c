#include <stdlib.h>
#include <stdio.h>
#include "pinning_mechanism.h"
#include "moves.h"
#include "lists.h"
#include "objects.h"

void    FindKingPosition(int *x, int *y){
    int king_x, king_y;
    if( season== WHITE_TO_MOVE ){
        ar_find(w_arsenal,  KING, &king_x, &king_y);
    }
    else{
        ar_find(b_arsenal, -KING, &king_x, &king_y);
    }
    *x = king_x;
    *y = king_y;
}

int     CheckIfPinPossbible(ANODE *p){
    int king_x, king_y;
    int x0 = p->coords[0];
    int y0 = p->coords[1];
    FindKingPosition(&king_x, &king_y);

    if( x0+y0==king_x+king_y ){
        return POS_DIAG;              // Pinned in the positive diagonal: " / "
    }
    else if( x0-y0==king_x-king_y){
        return NEG_DIAG;              // Pinned in the negative diagonal: " \ "
    }
    else if(y0==king_y){
        return HORIZONTAL;            // Pinned horizontally
    }
    else if( x0==king_x){
        return VERTICAL;              // Pinned vertically
    }
    else{
        return 0;
    }
}

int     MorePiecesInbetweenVertically(int x0, int y0, int king_y){
    int j;

    if( y0>king_y ){
        if( y0-king_y==1 ){ // Piece directly in front of king
            return 0;
        }
        else{
            for( j=king_y+1; j<y0; j++ ){
                if( board[x0][j]->piece!=EMPTY ){
                    printf("[INFO]: MORE PIECES IN FRONT OF KING VERTICALLY! (%d)\n", board[x0][j]->piece);
                    return 1;
                }
            }
            return 0;
        }
    }
    else{ // y0<king_y
        if( king_y-y0==1 ){
            return 0;
        }
        else{
            for( j=y0+1; j<king_y; j++ ){
                if( board[x0][j]->piece!=EMPTY ){
                    printf("[INFO]: MORE PIECES IN FRONT OF KING VERTICALLY! (%d)\n", board[x0][j]->piece);
                    return 1;
                }
            }
            return 0;
        }
    }
}

int     MorePiecesInbetweenHorizontally(int x0, int y0, int king_x){
    int i;

    if( x0>king_x ){
        if( x0-king_x==1 ){ // Piece directly in front of king
            return 0;
        }
        else{
            for( i=king_x+1; i<x0; i++ ){
                if( board[i][y0]->piece!=EMPTY ){
                    printf("[INFO]: MORE PIECES IN FRONT OF KING HORIZONTALLY! (%d)\n", board[i][y0]->piece);
                    return 1;
                }
            }
            return 0;
        }
    }
    else{ // x0<king_x
        if( king_x-x0==1 ){
            return 0;
        }
        else{
            for( i=x0+1; i<king_x; i++ ){
                if( board[i][y0]->piece!=EMPTY ){
                    printf("[INFO]: MORE PIECES IN FRONT OF KING HORIZONTALLY! (%d)\n", board[i][y0]->piece);
                    return 1;
                }
            }
            return 0;
        }
    }
}

int     MorePiecesInbetweenDiagonally(int x0, int y0, int king_x, int king_y, int slope_factor){
    int i, j;

    j = y0-slope_factor; // Slope factor is +1 fro positive diagonal " / " and -1 for negative " \ "
    if( x0>king_x ){
        if( x0-king_x==1 ){
            return 0;
        }
        else{
            for( i=king_x+1; i<x0; i++){
                if( board[i][j]->piece!=EMPTY ){
                    printf("[INFO]: MORE PIECES IN FRONT OF KING DIAGONALLY! (%d)\n", board[i][j]->piece);
                    return 1;
                }
                j-=slope_factor;
            }
            return 0;
        }
    }
    else{  // x0<king_x
        if( king_x-x0==1 ){
            return 0;
        }
        else{
            for( i=x0+1; i<king_x; i++){
                if( board[i][j]->piece!=EMPTY ){
                    printf("[INFO]: MORE PIECES IN FRONT OF KING DIAGONALLY! (%d)\n", board[i][j]->piece);
                    return 1;
                }
                j-=slope_factor;
            }
            return 0;
        }
    }
}


int     MorePiecesInFrontOfKing(int x0, int y0, int king_x, int king_y, int factor){
    int i, j;

    if( x0==king_x ){
        return MorePiecesInbetweenVertically(x0, y0, king_y);
    }
    else if( y0==king_y ){
        return MorePiecesInbetweenHorizontally(x0, y0, king_x);
    }
    else{
        return MorePiecesInbetweenDiagonally(x0, y0, king_x, king_y, factor);
    }
}

int     CheckDiagonalPin(ANODE *p, int factor){
    int i, j, king_x, king_y;
    int x0 = p->coords[0];
    int y0 = p->coords[1];
    int season_factor = (season==WHITE_TO_MOVE) ? (-1) : 1;
    FindKingPosition(&king_x, &king_y);

    if( x0>king_x && y0!=king_y ){
        i = x0+1;
        j = y0-factor;
        while( i<8 && ((j<8)&&(j>=0)) ){
            if( board[i][j]->piece != 0 ){
                if( (board[i][j]->piece==season_factor*BISHOP) || (board[i][j]->piece==season_factor*QUEEN) ){
                    if( MorePiecesInFrontOfKing(x0, y0, king_x, king_y, factor) ){
                        return 0;
                    }
                    else{
                        printf("[INFO]: Piece (%d) diagonal pin! (%d,%d) and King: (%d,%d)\n", p->piece, p->coords[0],p->coords[1], king_x, king_y);
                        return 1;
                    }
                }
                break;
            }
            i++;
            j-=factor;
        }
        return 0;
    }
    else if( x0<king_x && y0!=king_y ){
        i = x0-1;
        j = y0+factor;
        while( i>=0 && ((j<8)&&(j>=0)) ){
            printf("\t\t[INFO]: Board(%d,%d)=(%d) check for pin of %d at (%d,%d)\n", i,j,board[i][j]->piece,p->piece, p->coords[0],p->coords[1]);
            if( board[i][j]->piece != 0 ){
                if( (board[i][j]->piece==season_factor*BISHOP) || (board[i][j]->piece==season_factor*QUEEN) ){
                    if( MorePiecesInFrontOfKing(x0, y0, king_x, king_y, factor) ){
                        return 0;
                    }
                    else{
                        printf("[INFO]: Piece (%d) DIAGONAL pin! (%d,%d) and King: (%d,%d)\n", p->piece, p->coords[0],p->coords[1], king_x, king_y);
                        return 1;
                    }
                }
                break;
            }
            i--;
            j+=factor;
        }
        printf("\n");
        return 0;
    }
    else{
        return 0;
    }
}

int     CheckHorizontalPin(ANODE *p){
    int i, j, king_x, king_y;
    int x0 = p->coords[0];
    int y0 = p->coords[1];
    int season_factor = (season==WHITE_TO_MOVE) ? (-1) : 1;
    FindKingPosition(&king_x, &king_y);
    
    j = y0;
    i = (x0>king_x) ? (x0+1) : (x0-1);
    while( (i<8) && (i>=0) ){
        if( board[i][j]->piece != 0 ){
            if( (board[i][j]->piece==season_factor*ROOK) || (board[i][j]->piece==season_factor*QUEEN) ){
                if( MorePiecesInFrontOfKing(x0, y0, king_x, king_y, 1) ){
                    return 0;
                }
                else{
                    printf("[INFO]: Piece (%d) HORIZONTAL pin! (%d,%d) and King: (%d,%d) pinned by (%d) at (%d, %d)\n", p->piece, p->coords[0],p->coords[1], king_x, king_y,board[i][j]->piece,i,j);
                    return 1;
                }
            }
            break;
        }
        i = (x0>king_x) ? (i+1) : (i-1);
    }
    return 0;
}

int     CheckVerticalPin(ANODE *p){
    int i, j, king_x, king_y;
    int x0 = p->coords[0];
    int y0 = p->coords[1];
    int season_factor = (season==WHITE_TO_MOVE) ? (-1) : 1;
    FindKingPosition(&king_x, &king_y);

    i = x0;
    j = (y0>king_y) ? (y0+1) : (y0-1);
    while( (j<8) && (j>=0) ){
        if( board[i][j]->piece != 0 ){
            if( (board[i][j]->piece==season_factor*ROOK) || (board[i][j]->piece==season_factor*QUEEN) ){
                if( MorePiecesInFrontOfKing(x0, y0, king_x, king_y, 1) ){
                    return 0;
                }
                else{
                    printf("[INFO]: Piece (%d) VERTICAL pin! (%d,%d) and King: (%d,%d)\n", p->piece, p->coords[0],p->coords[1], king_x, king_y);
                    return 1;
                }
            }
            break;
        }
        j = (y0>king_y) ? (j+1) : (j-1);
    }
    return 0;
}

int    get_pin_direction(ANODE *p){                
    int king_x, king_y;
    int pin_direction = CheckIfPinPossbible(p);
    int pinned_positive_diagonal = 0;
    int pinned_negative_diagonal = 0;
    int pinned_horizontally      = 0;
    int pinned_vertically        = 0;
//                 [0]
//                  |
//          [7]     |     [4]
//             \    |    /
//                \ | /
//    [3]-----------------------[2]
//                / | \.
//             /    |    \.
//          [5]     |     [6]
//                  |
//                 [1]
    FindKingPosition(&king_x, &king_y);                              
                                                                     
    if( pin_direction!=0 ){                                          
        if( pin_direction==POS_DIAG ){
            pinned_positive_diagonal = CheckDiagonalPin(p, 1);  // Factor = 1 for positive diagonal "/"
            if( pinned_positive_diagonal ){
                if( king_x<p->coords[0] ){
                    return UPPER_RIGHT;
                }
                else{
                    return LOWER_LEFT;
                }
            }
        }
        else if( pin_direction==NEG_DIAG ){
            pinned_negative_diagonal = CheckDiagonalPin(p, -1); // Factor = -1 to check the negative diagonal "\"
            if( pinned_negative_diagonal ){
                if( king_x<p->coords[0] ){
                    return LOWER_RIGHT;
                }
                else{
                    return LOWER_LEFT;
                }
            }
        }
        else if( pin_direction==HORIZONTAL){
            pinned_horizontally = CheckHorizontalPin(p);
            if( pinned_horizontally ){
                if( king_x<p->coords[0] ){
                    return RIGHT;
                }
                else{
                    return LEFT;
                }
            }
        }
        else{
            pinned_vertically = CheckVerticalPin(p);
            if( pinned_vertically ){
                if( king_y>p->coords[1] ){
                    return UP;
                }
                else{
                    return DOWN;
                }
            }
        }
        return -1;
    }
    else{ // Piece is not pinned in any direction
        printf("~~~~~~~~~~~~~~~~~~~~~~~EDW\n");
        return -1;
    }
}

int     IsPinned(ANODE *p){
    int pinDirection = CheckIfPinPossbible(p);
    int factor;
    int pinResult;
    
    if( pinDirection==POS_DIAG ){
        factor = 1;
        pinResult = CheckDiagonalPin(p, factor); // Set to 1 to check the positive diagonal "/"
    }
    else if( pinDirection==NEG_DIAG ){
        factor = -1;
        pinResult = CheckDiagonalPin(p, factor); // Set to -1 to check the negative diagonal "\"
    }
    else if( pinDirection==HORIZONTAL ){
        pinResult = CheckHorizontalPin(p);
    }
    else if( pinDirection==VERTICAL ){
        pinResult = CheckVerticalPin(p);
    }
    else{
        pinResult = 0;
    }
    return pinResult;
}