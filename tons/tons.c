#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "list.h"

int **adj, **dist, n;
List **coul;

int**
initmat(int n)
{
	int i, **mat;

	mat = emalloc(n * sizeof(int*));
	for(i = 0; i < n; i++)
		mat[i] = ecalloc(n, sizeof(int));
	return mat;
}

List**
initlc(int n)
{
	List **lc;

	lc = ecalloc(n, sizeof(List*));
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
gengraphcycle(int n)
{
	int i, j;

	for(i = 0; i < n; i++){
		j = (i+1) % n;
		adj[i][j] = adj[j][i] = 1;
	}
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
bfs(int s, int *dist)
{
	int x, y, vis[n];
	List *q;	/* file */

	memset(vis, -1, n * sizeof(int));
	vis[s] = dist[s] = 0;
	q = newitem(s);
	while(q != NULL){
		x = q->val;
		q = delitem(q, 0);
		for(y = 0; y < n; y++){
			if(adj[x][y] && vis[y] == -1){
				q = addend(q, newitem(y));
				vis[y] = 1;
				dist[y] = dist[x]+1;
			}
		}
	}
}

void
initdist(int **dist)
{
	int i;

	/* calcul distances pour chaque sommet */
	for(i = 0; i < n; i++)
		bfs(i, dist[i]);
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
prtlc(List **lc)
{
	int i;

	for(i = 0; i < n; i++){
		printf("coul %d : ", i);
		apply(lc[i], printv, "%d ");
		printf("\n");
	}
}

/* |phi(x) intersection phi(y)| < d(x, y)*/
int
condtons(List **coul, int x, int c)
{
	int y;
	List *p;

	if((p = lookup(coul[x], c)) != NULL)
		return 0;
	for(y = 0; y < n; y++){
		if(x == y)
			continue;
		if((cardinter(coul[x], coul[y]) > dist[x][y]))
			return 0;
	}
	return 1;
}

int
convient(List **coul, int x, int c)
{
	int y;
	List *p;

	if(condtons(coul, x, c) == 0)
		return 0;
	for(y = 0; y < n; y++)
		if(adj[x][y] && ((p = lookup(coul[y], c)) != NULL))
			return 0;
	return 1;
}

/* coloration gloutonne */
int
colorsom(List **coul, int x)
{
	int c;

		for(c = 1; convient(coul, x, c) == 0; c++)
			;
		coul[x] = addend(coul[x], newitem(c));

	return c;
}

/* k-coloration, backtracking */
void
colorback(List **coul, int x, int k, int *stop)
{
	int c;

	if(x == n)
		*stop = 1;
	else{
		for(c = 1; c <= k; c++){
			if(convient(coul, x, c)){
				//printf("coul som %d %d\n", x, c);
				coul[x] = addfront(coul[x], newitem(c));
				colorback(coul, x+1, k, stop);
				if(*stop)
					return;
				coul[x] = delitem(coul[x], 0);
			}
		}
	}
}

int
colexact(List **coul, int b)
{
	int i, k, stop;

	for(i = 0; i < b; i++)
		for(k = stop = 0; stop == 0; k++)
			colorback(coul, 0, k, &stop);
	return k-1;	/* k-colo, nb chromatique */
}

int
colorglouton(List **coul, int b)
{
	int i, j, c, nc;

	nc = 0;
	for(i = 0; i < b; i++){
		for(j = 0; j < n; j++)
			if((c = colorsom(coul, j)) > nc)
				nc = c;
	}
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
actuvois(int *dsat, List **coul)
{
	int i, j, c;

	for(i = 0; i < n; i++){
		/* compte coul voisines à i */
		for(j = 0, c = 0; j < n; j++)
			if(adj[i][j])
				apply(coul[j], count, &c);
		if(c > 0 && dsat[i] > -1)
			dsat[i] = c;	/* reste deg[i] si aucun voisin color */
	}
}

int
dsatur(List **coul, int b)
{
	int i, s, c, nc, soc, dsat[n], deg[n];

	initdeg(deg);
	for(i = 0; i < b; i++){
		memmove(dsat, deg, n * sizeof(int));
		actuvois(dsat, coul);
		for (soc = nc = 0; soc < n; soc++){
			s = maxdsat(dsat, deg);
			if((c = colorsom(coul, s)) > nc)
				nc = c;
			dsat[s] = -1;
			actuvois(dsat, coul);
			//prttabdsat(dsat, s, c);
		}
	}
	return nc;
}

void
freelc(List **lc)
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

void
exec(int (*algo)(List**, int), List **coul, int b, int v)
{
	int nchrom;

	initdist(dist);
	if(v)
		affgraph(n);
	nchrom = (*algo)(coul, b);
	if(v)
		prtlc(coul);
	printf("Nombre chromatique %d\n", nchrom);
}

int
main(int argc, char **argv)
{
	int i, b, v;

	srand((unsigned)time(NULL));
	b = 1;
	n = v = 0;
	if(argc < 2)
		fprintf(stderr, "usage: tons [-n N] [-p P] [-b B] [-vgde]\n");
	else{
		for(i = 0; i < argc; i++){
			if(memcmp(argv[i], "-n", 2) == 0){
				n = atoi(argv[++i]);
				adj = initmat(n);
				coul = initlc(n);
				dist = initmat(n);
				gengraphcycle(n);
			}else if(memcmp(argv[i], "-p", 2) == 0 && n)
				gengraph(n, atoi(argv[++i]));
			else if(memcmp(argv[i], "-v", 2) == 0)
				v = 1;
			else if(memcmp(argv[i], "-b", 2) == 0 && n)
				b = atoi(argv[++i]);
			else if(memcmp(argv[i], "-glouton", strlen(argv[i])) == 0 && n)
				exec(colorglouton, coul, b, v);
			else if(memcmp(argv[i], "-dsatur", strlen(argv[i])) == 0 && n)
				exec(dsatur, coul, b, v);
			else if(memcmp(argv[i], "-exact", strlen(argv[i])) == 0 && n)
				exec(colexact, coul, b, v);
		}
	}
	freelc(coul);
	freemat(adj);
	freemat(dist);
	return 0;
}
