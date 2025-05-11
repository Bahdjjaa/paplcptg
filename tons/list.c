#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

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

List*
newitem(int val)
{
	List *newp;

	newp = (List *)emalloc(sizeof(List));
	newp->val = val;
	newp->next = NULL;
	return newp;
}

List*
addfront(List *lp, List *newp)
{
	newp->next = lp;
	return newp;
}

List*
addend(List *lp, List *newp)
{
	List *p;

	if(lp == NULL)
		return newp;
	/* decalage du pointeur */
	for(p = lp; p->next != NULL; p = p->next)
		;
	p->next = newp;
	return lp;
}

List*
delitem(List *lp, int n)
{
	int i;
	List *p, *prev;

	prev = NULL;
	for(i = 0, p = lp; p != NULL; p = p->next, i++){
		if(i == n){
			if(prev == NULL)
				lp = p->next;
			else
				prev->next = p->next;
			free(p);
			return lp;
		}
		prev = p;
	}
	return NULL;
}


List*
lookup(List *lp, int val)
{
	for( ; lp != NULL; lp = lp->next)
		if(val == lp->val)
			return lp;
	return NULL;
}

void
apply(List *lp, void (*fn)(List*, void*), void *arg)
{
	for( ; lp != NULL; lp = lp->next)
		(*fn)(lp, arg);
}

void
printv(List *p, void *arg)
{
	char *fmt;

	fmt = (char*)arg;
	printf(fmt, p->val);
}

void
count(List *p, void *arg)
{
	int *i;

	i = (int*)arg;
	(*i)++;
}

int
cardinter(List *lg, List *ld)
{
	int i;
	List *p;

	/* |lg intersection ld| */
	for(i = 0; lg != NULL; lg = lg->next)
		for(p = ld ; p != NULL; p = p->next)
			if(lg->val == p->val)
				i++;
	return i;
}

void
freeall(List *lp)
{
	List *next;

	for( ; lp != NULL; lp = next){
		next = lp->next;
		free(lp);
	}
}
