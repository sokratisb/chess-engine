#include "lists.h"


#ifndef __MOVES__
#define __MOVES__

extern int white_king_has_moved;
extern int black_king_has_moved;
extern int white_left_rook_has_moved;
extern int black_left_rook_has_moved;
extern int white_right_rook_has_moved;
extern int black_right_rook_has_moved;

void    m_create_pawn_moves(ANODE *p);
void    m_create_knight_moves(ANODE *p);
void    m_create_bishop_moves(ANODE *p);
void    m_create_rook_moves(ANODE *p);
void    m_create_queen_moves(ANODE *p);
void    m_create_king_moves(ANODE *p);
void    m_print_all_moves(void);
void    m_delete_move_queue(ANODE *p);
int     m_move_dequeue(ANODE *Q);
int     m_check_diagonals_for_protection(int colour, MNODE *move, int *attx, int *atty);
int     m_check_rows_and_columns_for_protection(int colour, MNODE *move, int *attx, int *atty);
int     m_check_for_protection_by_knight(int colour, MNODE *move, int *attx, int *atty);
int     exclude_curr_piece_from_protectors(int *a, int *b, int *c);
int     m_look_for_protectors(int colour, MNODE *move, int *attx, int *atty);


#endif