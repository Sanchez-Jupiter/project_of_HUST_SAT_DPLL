#include "def.h"

void cnfparser(CLAUSE *root){
    CLAUSE *r = root->next;
    int cnt = 1;
    while(r != NULL){
        if(r->flag != -1){
            r = r->next;
            continue;
        }
        LITERAL_NODE *p = r->first->next;
        printf("Clause %d: ", cnt++);
        while(p != NULL){
            if(!p->flag) {
                p = p->next;
                continue;
            }
            printf("%d ", p->literal);
            p = p->next;
        }
        printf("\n");
        r = r->next;
    }
}

int literalnum(CLAUSE *cl){
    LITERAL_NODE *h = cl->first->next;
    int cnt = 0;
    while(h != NULL){
        if(h->flag){
            cnt++;
        }
        h = h->next;
    }
    return cnt;
}

LITERAL_NODE* single_clause(CLAUSE *root){
    CLAUSE *r = root->next;
    while(r != NULL){
        if(r->flag != -1){
            r = r->next;
            continue;
        }
        if(literalnum(r) == 1){
            LITERAL_NODE *q = r->first->next;
            while(q != NULL){
                if(q->flag){
                    return q;
                }
                q = q->next;
            }
        }
        r = r->next;
    }
    return NULL;
}

bool isempty(CLAUSE *root){
    CLAUSE *r = root->next;
    while(r != NULL){
        if(r->flag == -1){
            return false;
        }
        r = r->next;
    }
    return true;
}

bool emptyclause(CLAUSE *root){
    CLAUSE *r = root->next;
    while(r != NULL){
        if(r->flag != -1){
            r = r->next;
            continue;
        }
        if(literalnum(r) == 0){
            return true;
        }
        r = r->next;
    }
    return false;
}


void deleteclause(LITERAL_NODE *p, int val){
    p->clause_head->flag = abs(val);
}


void deleteliteral(LITERAL_NODE *p){
    p->flag = false;
}


int findbranch(CLAUSE * root, LITERAL_NODE *same_literal_head[], bool book[], int literal_num){//key part of upgrading
    int s = literal_num;
    int i;
    int maxL = 0;
    int res_val = 0;
    CLAUSE *r = root->next;
    while(r != NULL){
        if(r->flag != -1){
            r = r->next;
            continue;
        }
        int ln = literalnum(r);
        if(s < ln){
            s = ln;
        }
        r = r->next;
    }
    for(i = 1; i <= literal_num; i++){
        if(book[i])
            continue;
        int x = 0, y = 0;
        LITERAL_NODE *p = same_literal_head[i]->nextsame;
        while(p != NULL){
             if(p->clause_head->flag != -1){
                p = p->nextsame;
                continue;
             }
             if(literalnum(p->clause_head) == s){
                if(p->literal == i){
                    x++;
                }
                else {
                    y++;
                }
             }
             p = p->nextsame;
        }
        int L = (x + 1) * (y + 1);
        if(L > maxL){
            maxL = L;
            res_val = (x >= y) ? i : -i;
         }
    }
    int cnt = 0;
    for(i = 1; i <= literal_num; i++){
        if(!book[i])
            cnt++;
    }
    printf("%d\n", cnt);
    return res_val;
}


void backtrack_clause(CLAUSE *cl){
    cl->flag = -1;
}


void backtrack_literal(LITERAL_NODE *p){
    p->flag = true;
}


void recur(int val, CLAUSE *root, LITERAL_NODE *same_literal_head[], bool book[]){
    LITERAL_NODE *p = same_literal_head[abs(val)]->nextsame;
    while(p != NULL){
        if(p->literal == val && p->clause_head->flag == abs(val)){
            backtrack_clause(p->clause_head);
        }
        else if(p->literal == -val && p->clause_head->flag == -1){
            backtrack_literal(p);
        }
        p = p->nextsame;

    }
    book[abs(val)] = false;
    //printf("%d", val);
    /*
    printf("backtrack:(%d)\n", val);
    cnfparser(root);
    */
}


bool DPLL(CLAUSE *root, LITERAL_NODE *same_literal_head[], bool res[], bool book[], int literal_num){
    //printf("1");
    LITERAL_NODE *s_clause = single_clause(root);
    CHANGED *cd;
    cd = NULL;
    while(s_clause != NULL){
        int val = s_clause->literal;
        CHANGED *new_cd = (CHANGED *)malloc(sizeof(CHANGED));
        new_cd->val = val;
        new_cd->next = cd;
        cd = (CHANGED *)malloc(sizeof(CHANGED));
        cd = new_cd;
        if(val > 0) {
            res[val] = true;
        }
        else{
            res[-val] = false;
        }
        LITERAL_NODE *p = same_literal_head[abs(s_clause->literal)]->nextsame;
        book[abs(val)] = true;
        while(p != NULL){
            if(p->clause_head->flag != -1){
                p = p->nextsame;
                continue;
            }
            if(p->literal == val){//single clause
                deleteclause(p, val);
            }
            else if(p->literal + val == 0){
                deleteliteral(p);
            }
            p = p->nextsame;
        }
        /*
        printf("d%d:\n",val);
        cnfparser(root);
        */
        if(isempty(root)){
            return true;
        }
        else if(emptyclause(root)){
            CHANGED *k = cd;
            while(k != NULL){
                recur(k->val, root, same_literal_head, book);
                k = k->next;
            }
            return false;
        }

        s_clause = single_clause(root);
    }
    /*
    printf("a:\n");
    cnfparser(root);
    */
    int val = findbranch(root, same_literal_head, book, literal_num);
    //if let br be true
    if(val > 0) {
        res[val] = true;
    }
    else{
        res[-val] = false;
    }
    LITERAL_NODE *p = same_literal_head[abs(val)]->nextsame;
    book[abs(val)] = true;
    while(p != NULL){
        if(p->clause_head->flag != -1){
            p = p->nextsame;
            continue;
        }
        if(p->literal == val){
            deleteclause(p, val);
        }
        else if(p->literal + val == 0){
            deleteliteral(p);
        }
        p = p->nextsame;
    }
    /*
    printf("b:(%d)\n", val);
    cnfparser(root);
    */
    if(isempty(root)){
        return true;
    }
    else if(emptyclause(root)){
        CHANGED *k = cd;
        while(k != NULL){
            recur(k->val, root, same_literal_head, book);
            k = k->next;
        }
        /*
        printf("e(%d):\n", val);
        cnfparser(root);
        */
        return false;
    }
    if(DPLL(root, same_literal_head, res, book, literal_num)){
        return true;
    }
        //recur
    recur(val, root, same_literal_head, book);
        //the other situation

    if(val > 0) {
        res[val] = false;
    }
    else{
        res[-val] = true;
    }
    /*
    printf("b:(%d)\n", -val);
    cnfparser(root);
    */
    p = same_literal_head[abs(val)]->nextsame;
    book[abs(val)] = true;
    while(p != NULL){
        if(p->clause_head->flag != -1){
            p = p->nextsame;
            continue;
        }
        if(p->literal == -val){
            deleteclause(p, val);
        }
        else if(p->literal == val){
            deleteliteral(p);
        }
        p = p->nextsame;
    }
    if(isempty(root)){
        return true;
    }
    else if(emptyclause(root)){
        CHANGED *k = cd;
        recur(-val, root, same_literal_head, book);
        while(k != NULL){
            recur(k->val, root, same_literal_head, book);
            k = k->next;
        }
        /*
        printf("e(%d):\n", -val);
        cnfparser(root);
        */
        return false;
    }
    bool ans = DPLL(root, same_literal_head, res, book, literal_num);
    if(ans) return true;
    else {
        recur(-val, root, same_literal_head, book);
        CHANGED *k = cd;
        while(k != NULL){
            recur(k->val, root, same_literal_head, book);
            k = k->next;
        }
        return false;
    }

}


