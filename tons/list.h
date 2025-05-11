typedef struct List List;

struct List {
        int val;
        List *next;
};

void *emalloc(unsigned n);
void *ecalloc(unsigned nm, unsigned n);
List *newitem(int val);
List *addfront(List *lp, List *newp);
List *addend(List *lp, List *newp);
List *delitem(List *lp, int n);
List* lookup(List *lp, int val);
void apply(List *lp, void (*fn)(List*, void*), void *arg);
void printv(List *p, void *arg);
void count(List *p, void *arg);
int cardinter(List *lg, List *ld);
void freeall(List *lp);
void freelc(List **lc);
