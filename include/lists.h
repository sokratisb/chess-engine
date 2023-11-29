#ifndef __LISTS__
#define __LISTS__


typedef struct move_node{
    int old_c[2];
    int new_c[2];
    int strength;
    struct move_node *next;
    int lock;
}MNODE;


typedef struct arsenal_node{
    int piece;
    int coords[2];
    volatile int lock;
    struct arsenal_node *next;
    struct move_node *mhead;
    struct move_node *mtail;
}ANODE;


extern ANODE *w_arsenal;
extern ANODE *b_arsenal;
extern   int  w_grave[15];
extern   int  b_grave[15];
extern MNODE *played_moves_head;
extern MNODE *rounds_best_move;

int     ar_insert(int piece, int x, int y, int colour);
int     ar_delete(ANODE *arsenal_head, int x, int y);
void    ar_find(ANODE *head, int piece, int *x, int *y);
void    insert_in_grave(int piece);
void    enqueue_played_move(void);
void    find_last_played_move_coords(int *x, int *y);
void    print_played_moves(void);
void    MQ_insert(ANODE *p, int x, int y);

void    print_arsenals(void);


#endif