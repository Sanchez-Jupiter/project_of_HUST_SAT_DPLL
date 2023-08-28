#include "def.h"

LITERAL_NODE *same_literal_head[LITERAL_NUM_SIZE];
LITERAL_NODE *same_literal_pre[LITERAL_NUM_SIZE];
bool book[LITERAL_NUM_SIZE];
bool res[LITERAL_NUM_SIZE];
int main()
{
    freopen("test1.txt","r",stdin);
    int flag = 1;
    while(flag){
        int opt;//sat or hanidoku
        char str_p[5], str_cnf[5];// p and cnf
        int literal_num, clause_num;

        LITERAL_NODE *pre;
        CLAUSE *root, *prehead;

        root =(CLAUSE *)malloc(sizeof(CLAUSE));
        root->next = NULL;

        prehead = root;

        int i;

        printf("------SAT OR HANIDOKU-----\n");
        printf("1.SAT           2.HANIDOKU\n");
        scanf("%d",&opt);

        switch(opt){
            case 1:
                printf("input the cnf file:\n");

                scanf("%s%s%d%d",str_p, str_cnf, &literal_num, &clause_num);
                for(i = 1; i <= literal_num; i++){
                    same_literal_head[i] = (LITERAL_NODE *)malloc(sizeof(LITERAL_NODE));
                    same_literal_pre[i] = same_literal_head[i];
                }
                for(i = 0; i < clause_num; i++){
                    int lit = 1;

                    CLAUSE *head;

                    LITERAL_NODE *p;
                    p = (LITERAL_NODE *)malloc(sizeof(LITERAL_NODE));

                    p->next = NULL;
                    p->nextsame = NULL;
                    p->pre = NULL;
                    p->presame = NULL;

                    head = (CLAUSE *)malloc(sizeof(CLAUSE));
                    p->clause_head = head;
                    head->first = p;
                    head->next = NULL;
                    prehead->next = head;
                    head->pre = prehead;
                    head->flag = -1;

                    prehead = head;
                    pre = p;
                    while(lit){
                        scanf("%d", &lit);
                        if(!lit) break;
                        p = (LITERAL_NODE *)malloc(sizeof(LITERAL_NODE));
                        p->literal = lit;
                        p->flag = true;
                        p->pre = pre;
                        pre->next = p;

                        p->next = NULL;

                        same_literal_pre[abs(p->literal)]->nextsame = p;
                        p->presame = same_literal_pre[abs(p->literal)];

                        p->nextsame = NULL;

                        same_literal_pre[abs(p->literal)] = p;
                        pre = p;

                        p->clause_head = head;
                    }
                }
                for(i = 1; i <= literal_num; i++){
                    book[i] = false;
                }
                /*
                printf("origin:\n");
                cnfparser(root);
                */
                if(!DPLL(root, same_literal_head, res, book, literal_num)){
                    printf("impossible !\n");
                }
                else {
                    printf("answer:\n");
                    for(i = 1; i <= literal_num; i++){
                        if(res[i]){
                            printf("%d ", i);
                        }
                        else printf("%d ", -i);
                    }
                }
                scanf("%d",&opt);
                break;

            case 2:
                printf("not ready!\n");
                scanf("%d", &opt);
                break;

            case 0:
                printf("goodbye!\n");
                scanf("%d", &opt);
                flag = 0;
                break;

        }
    }

    return 0;
}

