#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>
#include <sys/time.h>
#include "visuals.h"
#include "objects.h"
#include "lists.h"
#include "barriers.h"
#include "moves.h"
#include "pinning_mechanism.h"



#define MAXT 200
#define MAX_MOVES 50

int N;
int game_over;
pthread_t threadIDs[MAXT];


void    put_pieces_to_initial_positions(void){
    int i ,j;
    for( i=0; i<8; i++ ){
        for( j=0; j<8; j++ ){
            if(j == 6){
                board[i][j]->piece =  PAWN;
            }
            else if(j == 1){
                board[i][j]->piece = -PAWN;
            }
            else
                board[i][j]->piece = EMPTY;
        }
    }
    board[0][0]->piece = -ROOK; board[7][0]->piece = -ROOK;
    board[0][7]->piece =  ROOK; board[7][7]->piece =  ROOK;
    board[1][0]->piece = -KNIGHT; board[6][0]->piece = -KNIGHT;
    board[1][7]->piece =  KNIGHT; board[6][7]->piece =  KNIGHT;
    board[2][0]->piece = -BISHOP; board[5][0]->piece = -BISHOP;
    board[2][7]->piece =  BISHOP; board[5][7]->piece =  BISHOP;
    board[3][0]->piece =  -QUEEN; board[3][7]->piece = QUEEN;
    board[4][0]->piece =  -KING;  board[4][7]->piece = KING;
}

void    assign_init_positions_and_arsenals(void){
    int i,j;
    int c[2];
    for( i=0; i<8; i++ ){
        for( j=0; j<8; j++ ){
            if(j == 6){
                board[i][j]->piece =  PAWN;
                ar_insert(PAWN, i, j, WHITE_TO_MOVE);
            }
            else if(j == 1){
                board[i][j]->piece = -PAWN;
                ar_insert(-PAWN, i, j, BLACK_TO_MOVE);
            }
            else
                board[i][j]->piece = EMPTY;
        }
    }
    board[0][0]->piece = -ROOK; board[7][0]->piece = -ROOK;
    board[0][7]->piece =  ROOK; board[7][7]->piece =  ROOK;
    ar_insert(-ROOK, 0, 0, BLACK_TO_MOVE);
    ar_insert(-ROOK, 7, 0, BLACK_TO_MOVE);
    ar_insert(ROOK, 0, 7, WHITE_TO_MOVE);
    ar_insert(ROOK, 7, 7, WHITE_TO_MOVE);

    board[1][0]->piece = -KNIGHT; board[6][0]->piece = -KNIGHT;
    board[1][7]->piece =  KNIGHT; board[6][7]->piece =  KNIGHT;
    ar_insert(-KNIGHT, 1, 0, BLACK_TO_MOVE);
    ar_insert(-KNIGHT, 6, 0, BLACK_TO_MOVE);
    ar_insert(KNIGHT, 1, 7, WHITE_TO_MOVE);
    ar_insert(KNIGHT, 6, 7, WHITE_TO_MOVE);

    board[2][0]->piece = -BISHOP; board[5][0]->piece = -BISHOP;
    board[2][7]->piece =  BISHOP; board[5][7]->piece =  BISHOP;
    ar_insert(-BISHOP, 2, 0, BLACK_TO_MOVE);
    ar_insert(-BISHOP, 5, 0, BLACK_TO_MOVE);
    ar_insert(BISHOP, 2, 7, WHITE_TO_MOVE);
    ar_insert(BISHOP, 5, 7, WHITE_TO_MOVE);

    board[3][0]->piece =  -QUEEN; board[3][7]->piece = QUEEN;
    board[4][0]->piece =  -KING;  board[4][7]->piece = KING;
    ar_insert(-QUEEN, 3, 0, BLACK_TO_MOVE);
    ar_insert(-KING, 4, 0, BLACK_TO_MOVE);
    ar_insert(QUEEN, 3, 7, WHITE_TO_MOVE);
    ar_insert(KING, 4, 7, WHITE_TO_MOVE);
}

void    remove_from_arsenal(MNODE *p){
    int i = p->new_c[0];
    int j = p->new_c[1];
    int result;

    MNODE *pt = rounds_best_move;

    if( board[i][j]->piece < 0 ){
        result = ar_delete( b_arsenal,i, j);
        if( result == 0 )
            printf("\tCaptured piece deletion failed\n");
    }
    else if( board[i][j]->piece > 0 ){
        result = ar_delete( w_arsenal,i, j);
        if( result == 0 )
            printf("\tCaptured piece deletion failed\n");
    }
}

void    update_board(MNODE *p, int showcase){
    int old_x = p->old_c[0];
    int old_y = p->old_c[1];
    int new_x = p->new_c[0];
    int new_y = p->new_c[1];

    if( !showcase )
        remove_from_arsenal(p);
    board[new_x][new_y]->piece = board[old_x][old_y]->piece;
    board[old_x][old_y]->piece = 0;
}

void    init_board(void){
    int i,j;
    for( i=0; i<8; i++ ){
        for( j =0; j<8; j++ ){
            board[i][j] = malloc(sizeof(SQUARE));
        }
    }
    assign_init_positions_and_arsenals();
}


int    get_self_ID(void){
    int i;
    pthread_t real_ID = pthread_self();
    for( i=0; i<MAXT; i++){
        if( real_ID == threadIDs[i] )
            return i;
    }
}

void    create_piece_move_pool(ANODE *p){
    int piece = p->piece;
    if( (piece == PAWN) || (piece == -PAWN) )
        m_create_pawn_moves(p);
    else if( (piece == KNIGHT) || (piece == -KNIGHT) )
        m_create_knight_moves(p);
    else if( (piece == BISHOP) || (piece == -BISHOP) )
        m_create_bishop_moves(p);
    else if( (piece == ROOK) || (piece == -ROOK) )
        m_create_rook_moves(p);
    else if( (piece == QUEEN) || (piece == -QUEEN) )
        m_create_queen_moves(p);
    else if( (piece == KING) || (piece == -KING) )
        m_create_king_moves(p);
    else
        printf("\n\tError create_piece_move_pool! Not an actual piece (%d)!\n\n", piece);
}

int     isInCheck(void){
    int king_pos_X, king_pos_Y;
    int check_result = 0;
    FindKingPosition(&king_pos_X, &king_pos_Y);
    check_result = m_check_for_attackers(king_pos_X, king_pos_Y);
    if( check_result!=0 ){
        printf("[INFO]: Check happened! (%d)\n", check_result);
    }
}

MNODE    *select_a_move_from_pool(ANODE *p){
    int lock_value;
    MNODE *move = p->mhead;
    while( move != p->mtail ){
        lock_value = atomic_flag_test_and_set(&(move->lock));
        if( lock_value == 0 ){
            return move;
        }
        move = move->next;
    }
    return NULL;
}

int    flip_coin(void){
    int value;
    value = rand() % 2;
    return value;
}

void    suggest_current_move(MNODE *move){
    int i, j; int str;
    i = move->new_c[0];
    j = move->new_c[1];
    str = move->strength;

    rounds_best_move->strength = str;
    rounds_best_move->old_c[0] = move->old_c[0];
    rounds_best_move->old_c[1] = move->old_c[1];
    rounds_best_move->new_c[0] = i;
    rounds_best_move->new_c[1] = j;
}

void    compare_and_update_rounds_best(MNODE *move){
    int bad_move_flag = 0;
    int value = rand();
    int i = move->new_c[0]; int j = move->new_c[1];
    int q_piece = board[ move->old_c[0] ][ move->old_c[1] ]->piece;
    int str = board[i][j]->piece;
    int protected = m_check_for_attackers(i,j);
    
    if( protected !=0 ){
        // If capturing a defended piece of equal importance
        // assign some value less than capturing a free pawn
        // Also if pawn push to protected square -> go for it
        if( (str == -q_piece) && ((q_piece!=PAWN) || (q_piece != -PAWN)) )
            str = PAWN/2;
        else if( (q_piece == PAWN || q_piece == -PAWN) && (str == 0) )
            str = 0;  
        else if(  ( (str > (-q_piece)) && (season==WHITE_TO_MOVE) )  ||
                  ( (str < (-q_piece)) && (season==BLACK_TO_MOVE) )     ){
            str = 0;
            bad_move_flag = 1;
        }

    }
    if( !bad_move_flag )
        str += season ? (value%6) : -(value%6);
    move->strength = str;


    if( (str <= rounds_best_move->strength) && (season == WHITE_TO_MOVE) )
        suggest_current_move(move);
    else if( (str >= rounds_best_move->strength) && (season == BLACK_TO_MOVE) )
        suggest_current_move(move);

}

void    compare_each_moves_strength(void){
    ANODE *piece; MNODE *move;
    if( season == BLACK_TO_MOVE )
        piece = b_arsenal;
    else
        piece = w_arsenal;

    while( piece!=NULL ){
        move = piece->mhead;
        while( move != NULL ){
            move = select_a_move_from_pool(piece);
            if( move != NULL ){
                while( atomic_flag_test_and_set(&(rounds_best_move->lock)) ){;}
                compare_and_update_rounds_best(move);
                rounds_best_move->lock = 0;
            }
        }

        piece = piece->next;
    }
}

void    update_arsenal(void){
    ANODE *piece;
    if( season == BLACK_TO_MOVE )
        piece = b_arsenal;
    else
        piece = w_arsenal;
    while( piece!=NULL ){
        if(piece->coords[0]==rounds_best_move->old_c[0] &&
           piece->coords[1]==rounds_best_move->old_c[1]     )
           break;
        piece = piece->next;
    }
    if( piece == NULL )
        printf("\tUpdate Arsenal Error!\n\n");
    piece->coords[0] = rounds_best_move->new_c[0];
    piece->coords[1] = rounds_best_move->new_c[1];
}

void    check_if_game_over(void){
    total_move_count++;
    if( total_move_count == MAX_MOVES ||
        rounds_best_move->strength > 500 ||
        rounds_best_move->strength < -500)
        game_over = 1;
}

void    wait_for_move_generation(void){
    pthread_barrier_wait(&barrier_move_generation);
}

void    wait_to_start_new_turn(void){
    pthread_barrier_wait(&barrier_new_turn);
}

void     choose_piece_and_init_moves(void){
    int TAS_res;
    ANODE *p;
    
    if( season == WHITE_TO_MOVE )
        p = w_arsenal;
    else
        p = b_arsenal;
    
    while(p != NULL){
        while( p!=NULL ){
            TAS_res = atomic_flag_test_and_set( &(p->lock) );
            if( TAS_res == 0 )
                break;
            else
                p = p->next;
        }
        if( p!=NULL ){
            create_piece_move_pool(p);
            p = p->next;
        }
    }
}

void    reset_move_queues(void){
    ANODE *p;
    if( season == WHITE_TO_MOVE )
        p = w_arsenal;
    else
        p = b_arsenal;
    
    while(p != NULL){
        while( p->mhead != p->mtail ){
            m_move_dequeue(p);
        }
        p = p->next;
    }
}

void    reset_current_move_str(void){
    rounds_best_move->strength = 0;
    first_best_move_flag = 0;
}

void    reset_pieces_locks(void){
    ANODE *p;
    if( season == WHITE_TO_MOVE )
        p = w_arsenal;
    else
        p = b_arsenal;
    while( p!=NULL ){
        p->lock = 0;
        p = p->next;
    }
}

void    count_remaining_moves_in_queues(void){
    ANODE *p; MNODE *m;
    int count = 0;
    if( season == WHITE_TO_MOVE )
        p = w_arsenal;
    else
        p = b_arsenal;
    
    while( p!=NULL ){
        count = 0;
        m = p->mhead;
        while( m != p->mtail ){
            count++;
            m = m->next;
        }
        p = p->next;
        if( count != 0 ){
            printf("\tCOUNT !=0 ERROR, MOVE QUEUES NOT DELETED!\n\n");
            game_over = 1;
        }

    }
}

void    execute_thread_functs(void){
    unsigned long elapsed_time;
    struct timeval start, finish;

    while( game_over == 0 ){
        choose_piece_and_init_moves();
        wait_for_move_generation(); // barrier    
        
        compare_each_moves_strength();
        pthread_barrier_wait(&barrier_played_move_nq);

        if( get_self_ID() == 0 ){
            enqueue_played_move();
            update_arsenal();
            isInCheck();
            check_if_game_over();
            reset_current_move_str();
            reset_pieces_locks();
            update_board(rounds_best_move, 0);
        }
        pthread_barrier_wait(&barrier_ID_0_bookeeping);

    // All threads delete pieces' move nodes
    // using Unbounded Lock-Free delete function (CAS used)
        reset_move_queues();    
        if( get_self_ID() == 0 ){
            count_remaining_moves_in_queues();
            season = !season;
        }

        wait_to_start_new_turn();
    }
    pthread_exit(0);
}

void    init_locks(void){
    pthread_barrier_init(&barrier_move_generation, NULL, N);
    pthread_barrier_init(&barrier_played_move_nq, NULL, N);
    pthread_barrier_init(&barrier_ID_0_bookeeping, NULL, N);
    pthread_barrier_init(&barrier_new_turn, NULL, N);
    pthread_barrier_init(&barrier_piece_lock, NULL, N);
    pthread_mutex_init(&game_over_lock, NULL);
}

void    create_N_threads(void){
    int i;
    for(i=0;i<N;i++){
        pthread_create(&(threadIDs[i]), NULL, &execute_thread_functs, NULL);
    }
}

void    join_all_threads(void){
    int i;
    for(i=0; i<N; i++)
        pthread_join(threadIDs[i], NULL);
}

void    init(void){
    rounds_best_move = malloc(sizeof(MNODE));
    rounds_best_move->strength = 0;
    rounds_best_move->lock = 0;
    game_over = 0;
    first_best_move_flag = 0;
    total_move_count = 0;
    season = WHITE_TO_MOVE;
    w_arsenal = NULL;
    b_arsenal = NULL;
    played_moves_head = NULL;
    // srand( (unsigned)time(NULL) );

    init_locks();
    init_board();
}

void    sync(void){
    sleep(1);
    al_flip_display();
}

void    print_played_moves(void){
    MNODE *p = played_moves_head;
    int count = 1;
    while( p!=NULL ){
        printf("%d.(%d,%d)->(%d,%d)\n",count,p->old_c[0],p->old_c[1],p->new_c[0],p->new_c[1]);
        count++;
        p=p->next;
    }
}

void    show_game(void){
    v_init();
    int i;
    MNODE *p = played_moves_head;

    while( p != NULL ){
        v_print_board();
        v_print_pieces();
        update_board(p, 1);
        sync();
        p = p->next;
    }
    v_print_board();
    v_print_pieces();
    sync();
    getchar();
}

int     main(int argc, char **argv){
    int i;
    unsigned long elapsed_time;
    struct timeval start, finish;
    if( argc != 2 ){
        printf("Error! Number of threads not passed as main argument! Exiting...\n");
        return 0;
    }else
        N = atoi(argv[1]);
    init();
    
    gettimeofday(&start, NULL);
    create_N_threads();
    join_all_threads();
    gettimeofday(&finish, NULL);
    elapsed_time = finish.tv_usec - start.tv_usec;
    print_played_moves();
    printf("\t\tElapsed time : %lu microseconds\n", elapsed_time);
    put_pieces_to_initial_positions();
    show_game();
    
    return 0; 
} 