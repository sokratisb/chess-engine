#include "barriers.h"

pthread_barrier_t barrier_move_generation;
pthread_mutex_t   game_over_lock;
pthread_barrier_t barrier_played_move_nq;
pthread_barrier_t barrier_ID_0_bookeeping;
pthread_barrier_t barrier_new_turn;
pthread_barrier_t barrier_piece_lock;