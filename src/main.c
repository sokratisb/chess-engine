#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "../include/engine.h"
#include "../include/objects.h"
#include "../include/player_VS_AI.h"


int     main(int argc, char **argv){
    int i;
    unsigned long elapsed_time;
    struct timeval start, finish;
    if( argc != 2 ){
        printf("Error! Number of threads not passed as main argument! Exiting...\n");
        return 0;
    }else
        N = atoi(argv[1]);
    // init();
    // gettimeofday(&start, NULL);
    // create_N_threads();
    // join_all_threads();
    // gettimeofday(&finish, NULL);
    // elapsed_time = finish.tv_usec - start.tv_usec;
    // print_played_moves();
    // printf("\t\tElapsed time : %lu microseconds\n", elapsed_time);
    // put_pieces_to_initial_positions();
    // if( game_over && total_move_count<MAX_MOVES ){
    //     show_game();
    // }

    init();
    for(i=0;i<N;i++){
        pthread_create(&(threadIDs[i]), NULL, &game_loop_p_vs_ai, NULL);
    }
    join_all_threads();
    return 0; 
}

// TODO: Discovered checks