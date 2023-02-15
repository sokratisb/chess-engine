#include "lists.h"


#ifndef __PINNING__
#define __PINNING__

enum    dir{POS_DIAG, NEG_DIAG, HORIZONTAL, VERTICAL};

void    FindKingPosition(int *x, int *y);
int     CheckIfPinPossbible(ANODE *p);

int     MorePiecesInbetweenVertically(int x0, int y0, int king_y);
int     MorePiecesInbetweenHorizontally(int x0, int y0, int king_x);
int     MorePiecesInbetweenDiagonally(int x0, int y0, int king_x, int king_y, int slope_factor);
int     MorePiecesInFrontOfKing(int x0, int y0, int king_x, int king_y, int factor);
int     CheckDiagonalPin(ANODE *p, int factor);
int     CheckHorizontalPin(ANODE *p);
int     CheckVerticalPin(ANODE *p);
int     IsPinned(ANODE *p);


#endif