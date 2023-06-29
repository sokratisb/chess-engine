#ifndef __ENGINE__
#define __ENGINE__

#include <pthread.h>
#include "lists.h"

#define MAXT 200

enum game_states{STATE_AI_TO_MOVE,
                 STATE_AI_INIT_PLAYER_MOVES,
                 STATE_COLOUR_CHOICE,
                 STATE_CHOOSE_PIECE_TO_MOVE,
                 STATE_HIGHLIGHT_SQUARE_TO_MOVE_TO,
                 STATE_EXECUTE_PLAYED_MOVE,
                 STATE_RESET_PLAYER_MOVE_POOL,
                 STATE_GAME_OVER
};

extern int N;
extern pthread_t threadIDs[MAXT];

extern int player_colour;
extern int game_state;


void    put_pieces_to_initial_positions(void);
void    assign_init_positions_and_arsenals(void);
void    remove_from_arsenal(MNODE *p);
void    update_board(MNODE *p, int showcase);
void    init_board(void);
int     get_self_ID(void);
void    create_piece_move_pool(ANODE *p);
int     isInCheck(void);
MNODE   *select_a_move_from_pool(ANODE *p);
void    suggest_current_move(MNODE *move);
int     calc_if_move_blocks_check(MNODE *move, int attacker_x, int attacker_y);
int     is_covered_by_more_than_one_pieces(int piece_value);
int     compute_moves_strength(MNODE *move);
void    compare_and_update_rounds_best(MNODE *move);
void    compare_each_moves_strength(void);
void    update_arsenal(void);
void    check_if_game_over(void);
void    wait_for_move_generation(void);
void    wait_to_start_new_turn(void);
void    choose_piece_and_init_moves(void);
void    reset_move_queues(void);
void    reset_current_move_str(void);
void    reset_pieces_locks(void);
void    count_remaining_moves_in_queues(void);
int     is_checkmate(void);
void    execute_thread_functs(void);
void    init_locks(void);
void    create_N_threads(void);
void    join_all_threads(void);
void    init(void);
void    sync(void);
void    show_game(void);

#endif