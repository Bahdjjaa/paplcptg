#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "lcol.h"

int **adj, n;
Lcol **coul;

int**
initmat(int n)
{
	int i, **mat;

	mat = emalloc(n * sizeof(int*));
	for(i = 0; i < n; i++)
		mat[i] = ecalloc(n, sizeof(int));
	return mat;
}

Lcol**
initlc(int n)
{
	int i;
	Lcol **lc;

	lc = ecalloc(n, sizeof(Lcol*));
	return lc;
}

void
gengraph(int n, int p)
{
	int i, j;

	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			adj[i][j] = adj[j][i] = (rand() % 100 < p && i != j);
}

void
initdeg(int *deg)
{
	int i, j;

	memset(deg, 0, n * sizeof(int));
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			deg[i] += adj[i][j];
}

void
affgraph(int n)
{
	int i, j;

	for(i = 0; i < n; i++){
		printf("Voisins de %d : ", i);
		for(j = 0; j < n; j++){
			if(adj[i][j])
				printf("%d ", j);
		}
		printf("\n");
	}
}

void
prtlc(Lcol **lc)
{
	int i;

	for(i = 0; i < n; i++){
		printf("coul %d : ", i);
		apply(lc[i], printv, "%d ");
		printf("\n");
	}
}

int
convient(Lcol **coul, int x, int c)
{
	int i;
	Lcol *p;

	/* verif coulo des voisins de x */
	for(i = 0; i < n; i++)
		if(adj[x][i] && ((p = lookup(coul[i], c)) != NULL) && i != x)
			return 0;
	return 1;
}

/* coloration gloutonne */
int
colorsom(Lcol **coul, int x)
{
	int c;

	for(c = 1; convient(coul, x, c) == 0; c++)
		;
	coul[x] = addend(coul[x], newitem(c));	/* plus petite coul dispo */
	return c;
}

/* k-coloration, backtracking */
void
colorback(Lcol **coul, int x, int k, int *stop)
{
	int c;

	if(x == n)
		*stop = 1;
	else{
		for(c = 1; c <= k; c++){
			if(convient(coul, x, c)){
				//printf("coul som %d %d\n", x, c);
				coul[x] = c;
				colorback(coul, x+1, k, stop);
				if(*stop)
					return;
			}
		}
	}
}

int
colexact(Lcol **coul)
{
	int k, stop;

	for(k = stop = 0; k < n && stop == 0; k++){
		memset(coul, 0, n * sizeof(int));
		colorback(coul, 0, k, &stop);
		if(stop == 0)
			printf("Pas de coloration en %d couleurs\n", k);
	}
	return k-1;	/* k-colo, nb chromatique */
}

int
colorglouton(Lcol **coul)
{
	int i, c, nc;

	nc = 0;
	memset(coul, 0, n * sizeof(int));
	for(i = 0; i < n; i++)
		if((c = colorsom(coul, i)) > nc)
			nc = c;
	return nc;
}

/* affiche les etapes du dsatur */
void
prttabdsat(int *dsat, int s, int c)
{
	int i;

	printf("c(%d) = %d\t", s, c);
	for(i = 0; i < n; i++)
		printf("%d\t", dsat[i]);
	printf("\n");
}

int
maxdsat(int *dsat, int *deg)
{
	int i, s;

	for(s = 0, i = 0; i < n; i++)
		if(dsat[s] < dsat[i] || (dsat[s] == dsat[i] && deg[s] < deg[i]))
			s = i;
	return s;
}

void
actuvois(int *dsat, Lcol **coul)
{
	int i, j, c;

	for(i = 0; i < n; i++){
		/* compte coul voisines à i */
		for(j = 0, c = 0; j < n; j++)
			if(adj[i][j] && coul[j] > 0)
				c++;
		if(c > 0 && dsat[i] > -1)
			dsat[i] = c;	/* reste deg[i] si aucun voisin color */
	}
}

int
dsatur(Lcol **coul)
{
	int s, c, nc, soc, dsat[n], deg[n];

	initdeg(deg);
	memset(coul, 0, n * sizeof(int));
	memmove(dsat, deg, n * sizeof(int));
	for (soc = nc = 0; soc < n; soc++){
		s = maxdsat(dsat, deg);
		if((c = colorsom(coul, s)) > nc)
			nc = c;
		dsat[s] = -1;
		actuvois(dsat, coul);
		prttabdsat(dsat, s, c);
	}
	return nc;
}

void
freelc(Lcol **lc)
{
	int i;

	for(i = 0; i < n; i++)
		freeall(lc[i]);
	free(lc);
}

void
freemat(int **mat)
{
	int i;

	for(i = 0; i < n; i++)
		free(mat[i]);
	free(mat);
}

int
main()
{
	int i;

	n = 8;
	srand((unsigned)time(NULL));
	adj = initmat(n);
	gengraph(n, 50);
	affgraph(n);

	coul = initlc(n);
	coul[0] = addend(coul[0], newitem(11));
	apply(coul[0], printv, "%d ");

	freelc(coul);
	freemat(mat);
//	colorglouton(coul);
//	prtlc(coul);

	/*printf("nbchroma : %d\n", colexact(coul1));
	for(i = 0; i < n; i++)
		printf("couleur de %d : %d\n", i, coul1[i]);
	colorglouton(coul2);
	for(i = 0; i < n; i++)
		printf("(glout) col %d : %d deg %d\n", i, coul2[i], deg[i]);
	printf("nb coul (dsatur) : %d\n", dsatur(coul3));*/
	return 0;
}
