#include "lists.h"


#ifndef __MOVES__
#define __MOVES__

void    m_create_pawn_moves(ANODE *p);
void    m_create_knight_moves(ANODE *p);
void    m_create_bishop_moves(ANODE *p);
void    m_create_rook_moves(ANODE *p);
void    m_create_queen_moves(ANODE *p);
void    m_create_king_moves(ANODE *p);
void    m_print_all_moves(void);
void    m_delete_move_queue(ANODE *p);
int     m_move_dequeue(ANODE *Q);
int     m_check_diagonals_for_attacker(int i, int j);
int     m_check_rows_and_columns_for_attacker(int i, int j);
int     m_check_for_attack_by_knight(int i, int j);
int     m_check_for_attackers(int i, int j);


#endif