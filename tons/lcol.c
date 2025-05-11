#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcol.h"

void*
emalloc(unsigned n)
{
	char *p;

	if((p = malloc(n)) == 0){
		fprintf(stderr, "out of memory");
		exit(1);
	}
	return p;
}

void*
ecalloc(unsigned nm, unsigned n)
{
	char *p;

	if((p = calloc(nm, n)) == 0){
		fprintf(stderr, "out of memory");
		exit(1);
	}
	return p;
}

Lcol*
newitem(int val)
{
	Lcol *newp;

	newp = (Lcol *)emalloc(sizeof(Lcol));
	newp->val = val;
	newp->next = NULL;
	return newp;
}

Lcol*
addfront(Lcol *lp, Lcol *newp)
{
	newp->next = lp;
	return newp;
}

Lcol*
addend(Lcol *lp, Lcol *newp)
{
	Lcol *p;

	if(lp == NULL)
		return newp;
	/* decalage du pointeur */
	for(p = lp; p->next != NULL; p = p->next)
		;
	p->next = newp;
	return lp;
}

Lcol*
lookup(Lcol *lp, int val)
{
	for( ; lp != NULL; lp = lp->next)
		if(val == lp->val)
			return lp;
	return NULL;
}

void
apply(Lcol *lp, void (*fn)(Lcol*, void*), void *arg)
{
	for( ; lp != NULL; lp = lp->next)
		(*fn)(lp, arg);
}

void
printv(Lcol *p, void *arg)
{
	char *fmt;

	fmt = (char*)arg;
	printf(fmt, p->val);
}

void
count(Lcol *p, void *arg)
{
	int *i;

	i = (int*)arg;
	(*i)++;
}

void
freeall(Lcol *lp)
{
	Lcol *next;

	for( ; lp != NULL; lp = next){
		next = lp->next;
		free(lp);
	}
}
