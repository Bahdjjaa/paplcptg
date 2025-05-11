typedef struct Lcol Lcol;

struct Lcol {
        int val;
        Lcol *next;
};

void *emalloc(unsigned n);
void *ecalloc(unsigned nm, unsigned n);
Lcol *newitem(int val);
Lcol *addfront(Lcol *lp, Lcol *newp);
Lcol *addend(Lcol *lp, Lcol *newp);
Lcol* lookup(Lcol *lp, int val);
void apply(Lcol *lp, void (*fn)(Lcol*, void*), void *arg);
void printv(Lcol *p, void *arg);
void count(Lcol *p, void *arg);
void prtlc(Lcol **lc, int n);
void freeall(Lcol *lp);
void freelc(Lcol **lc, int n);
