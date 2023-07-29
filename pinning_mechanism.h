#include "lists.h"


#ifndef __PINNING__
#define __PINNING__

enum    dir{POS_DIAG=1, NEG_DIAG=2, HORIZONTAL=3, VERTICAL=4};
enum    pin_dir{UP  =1, DOWN =2,
                LEFT=3, RIGHT=4,
                UPPER_RIGHT=5, LOWER_LEFT =6,
                UPPER_LEFT =8, LOWER_RIGHT=7};

void    FindKingPosition(int *x, int *y);
int     CheckIfPinPossbible(ANODE *p);

int     MorePiecesInbetweenVertically(int x0, int y0, int king_y);
int     MorePiecesInbetweenHorizontally(int x0, int y0, int king_x);
int     MorePiecesInbetweenDiagonally(int x0, int y0, int king_x, int king_y, int slope_factor);
int     MorePiecesInFrontOfKing(int x0, int y0, int king_x, int king_y, int factor);
int     CheckDiagonalPin(ANODE *p, int factor);
int     CheckHorizontalPin(ANODE *p);
int     CheckVerticalPin(ANODE *p);
int     get_pin_direction(ANODE *p);
int     IsPinned(ANODE *p);


#endif