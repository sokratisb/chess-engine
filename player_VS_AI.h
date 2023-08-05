#ifndef __PLAYER_VS_AI__
#define __PLAYER_VS_AI__

extern ANODE *curr_touched_piece;

void    reset_queues(void);
void    execute_AI(void);
void    get_player_colour_choice(int mx, int my);
void    get_player_piece_choice(int mx, int my);
int     mark_squares_from_move_pool(int x, int y);
void    reset_board_colouring(void);
void    highlight_available_squares(void);
ANODE  *find_players_chosen_piece(void);
void    execute_move(int new_x, int new_y);
void    set_promotion_choice_buttons(int x, int y);
void    try_to_execute_move(int mx, int my);
void    execute_promotion(int mx, int my);
void    execute_player_action(void);
void    game_loop_p_vs_ai(void);
void    render_colour_choices(void);
void    init_p_vs_ai(void);


#endif