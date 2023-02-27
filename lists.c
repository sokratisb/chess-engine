#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "lists.h"
#include "objects.h"

void    MQ_insert(ANODE *p, int x, int y){
    MNODE *m = malloc(sizeof(MNODE));
    if( m==NULL )
        printf("Malloc ERROR!\n\n");
    m->old_c[0] = p->coords[0];
    m->old_c[1] = p->coords[1];
    m->new_c[0] = x;
    m->new_c[1] = y;
    m->lock = 0;
    m->next = p->mhead;
    p->mhead = m;
}

int   ar_insert(int piece, int x, int y, int colour){
    ANODE *p = malloc(sizeof(ANODE));
    if( p==NULL )
        printf("Malloc ERROR!\n\n");

    if( colour == WHITE_TO_MOVE ){
        p->next = w_arsenal;
        w_arsenal = p;
    }else{
        p->next = b_arsenal;
        b_arsenal = p;
    }
    p->coords[0] = x;
    p->coords[1] = y;
    p->piece = piece;
    p->lock  = 0;

    p->mhead = malloc(sizeof(MNODE));
    p->mtail = p->mhead;
    p->mhead->next = NULL;
    p->mhead->lock = 0;
    p->mhead->new_c[0] = -1;
    p->mhead->new_c[1] = -1;
    p->mhead->old_c[0] = -1;
    p->mhead->old_c[1] = -1;
    p->mhead->strength = -1;

    return 1;
}

int     ar_delete(ANODE *arsenal_head, int x, int y){
    ANODE *curr;
    ANODE *pred = NULL;

    curr = arsenal_head;
    while( curr != NULL ){
        if( curr->coords[0] == x && curr->coords[1] == y ){
            if( pred == NULL ){
                if( arsenal_head==w_arsenal ){
                    w_arsenal = w_arsenal->next;
                }
                else{
                    b_arsenal = b_arsenal->next;
                }
            }
            else
                pred->next = curr->next;
            free(curr);
            return 1;
        }
        else{
            pred = curr;
            curr = curr->next;
        }
    }
    printf("failed to delete %d,%d\n",x,y);
    return 0;

}

void    ar_find(ANODE *head, int piece, int *x, int *y){
    ANODE *ptr = head;
    while( ptr->piece!=piece && ptr!=ptr->mtail ){
        ptr = ptr->next;
    }
    if( ptr->piece==piece ){
        *x = ptr->coords[0];
        *y = ptr->coords[1];
    }
    else{
        printf("[Error]: Piece (%d) not found\n", piece);
    }
}


void    print_arsenals(void){
    ANODE *p = w_arsenal;
    while(p!= NULL){
        printf("\t%d -> [%d, %d]\n", p->piece, p->coords[0], p->coords[1]);
        p = p->next;
    }
    p = b_arsenal;
    while(p!= NULL){
        printf("\t\t%d -> [%d, %d]\n", p->piece, p->coords[0], p->coords[1]);
        p = p->next;
    }
}

void    enqueue_played_move(void){
    MNODE *prev = played_moves_head;
    MNODE *p = malloc(sizeof(MNODE));
    if( p==NULL )
        printf("Malloc ERROR!\n\n");
    p->old_c[0] = rounds_best_move->old_c[0];
    p->old_c[1] = rounds_best_move->old_c[1];
    p->new_c[0] = rounds_best_move->new_c[0];
    p->new_c[1] = rounds_best_move->new_c[1];
    p->lock = 0;
    p->next = NULL;
    p->strength = 0;

    if( played_moves_head == NULL )
        played_moves_head = p;
    else{
        while( prev->next !=NULL ){
            prev = prev->next;
        }
        prev->next = p;
    }
}

void    find_last_played_move_coords(int *x, int *y){
    MNODE *ptr = played_moves_head;

    if( ptr==NULL ){
        fprintf(stderr, "[Error]: Looking for played move when no move has been played!\n\n");
    }
    while( ptr->next!=NULL ){
        ptr=ptr->next;
    }
    *x = ptr->new_c[0];
    *y = ptr->new_c[1];
}