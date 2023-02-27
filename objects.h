#ifndef __OBJECTS__
#define __OBJECTS__


#define CENTER 40
#define MAX_MOVES 100
#define RANDOM_FACTOR 30


enum type{EMPTY  = 0,
          PAWN   = 100, KNIGHT = 300,
          BISHOP = 310, ROOK   = 500,
          QUEEN  = 900, KING   = 9000};

enum turn{WHITE_TO_MOVE=0, BLACK_TO_MOVE=1 };
enum piece_colour{WHITE_PIECE=0, BLACK_PIECE=1};
enum check{SIMPLE_CHECK=1, DOUBLE_CHECK=2};

enum move_values{MOVE_OUT_OF_CHECK           = 150,
                 CAPTURE_KINGS_ATTACKER      = 250,
                 BLOCK_CHECK_DEFENDED        = 200,
                 BLOCK_CHECK_UNDEFENDED      = 50,
                 BLOCK_CHECK_DOUBLE_ATTACKED = 50,
                 CAPTURE_UNDEFENDED_PIECE    = 250,
                 TRADE_EQUAL_PIECES          = 70,
                 TRADE_UP                    = 200,
                 MOVE_TO_WHITE_SPACE         = 100};

typedef struct board_square{
    int piece;
}SQUARE;

int season;
int game_over;
int total_move_count;
int first_best_move_flag;
int defend_from_check_flag;
SQUARE *board[8][8];

#endif