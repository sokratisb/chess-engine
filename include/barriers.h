#include <pthread.h>

#ifndef __BARRIERS__
#define __BARRIERS__

extern pthread_barrier_t barrier_move_generation;
extern pthread_mutex_t game_over_lock;
extern pthread_barrier_t barrier_played_move_nq;
extern pthread_barrier_t barrier_ID_0_bookeeping;
extern pthread_barrier_t barrier_new_turn;
extern pthread_barrier_t barrier_piece_lock;

#endif