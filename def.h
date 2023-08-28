#ifndef DEF_H_INCLUDED
#define DEF_H_INCLUDED

#include<cstdio>
#include<malloc.h>

#define LITERAL_NUM_SIZE 10000



typedef struct clause{
    struct clause *next;
    struct clause *pre;
    struct node *first;
    int flag;// if it has been deleted
}CLAUSE;

typedef struct node{
    int literal;
    struct node *next;
    struct node *pre;
    struct node *nextsame;
    struct node *presame;
    CLAUSE *clause_head;
    bool flag;
}LITERAL_NODE;

typedef struct cd{
    int val;
    struct cd *next;
}CHANGED;

void cnfparser(CLAUSE *root);
int literalnum(CLAUSE *cl);
bool DPLL(CLAUSE *root, LITERAL_NODE *same_literal_head[], bool res[], bool book[], int literal_num);
bool isempty(CLAUSE *root);
bool emptyclause(CLAUSE *root);
void deleteclause(CLAUSE *p, int val);
void deleteliteral(LITERAL_NODE *p);
int findbranch(CLAUSE * root, LITERAL_NODE *same_literal_head[], bool book[]);
void backtrack_clause(CLAUSE *cl);
void backtrack_literal(LITERAL_NODE *p);
void recur(int val, CLAUSE *root, LITERAL_NODE *same_literal_head[], bool book[]);
#endif // DEF_H_INCLUDED

