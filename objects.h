#ifndef __OBJECTS__
#define __OBJECTS__


#define CENTER 40
#define MOVES 100


enum type{EMPTY  = 0,
          PAWN   = 10, KNIGHT = 30,
          BISHOP = 31, ROOK   = 50,
          QUEEN  = 90, KING   = 900};

enum turn{ WHITE_TO_MOVE = 0, BLACK_TO_MOVE = 1 };


typedef struct board_square{
    int piece;
}SQUARE;

int season;
int total_move_count;
int first_best_move_flag;
int defend_from_check_flag;
SQUARE *board[8][8];

#endif