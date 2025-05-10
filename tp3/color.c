#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

enum {
	N = 8,
};

int adj[N][N];	/* matrice d'adjacences */
int deg[N], coul1[N], coul2[N], coul3[N];

void
gengraph(int n, int p)
{
	int i, j;

	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			adj[i][j] = adj[j][i] = (rand() % 100 < p && i != j);
}

void
initdeg()
{
	int i, j;

	memset(deg, 0, N * sizeof(int));
	for(i = 0; i < N; i++)
		for(j = 0; j < N; j++)
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

int
convient(int *coul, int x, int c)
{
	int i;

	/* verif coul des voisins de x */
	for(i = 0; i < N; i++)
		if(adj[x][i] && (coul[i] == c) && i != x)
			return 0;
	return 1;
}

/* coloration gloutonne */
int
colorsom(int *coul, int x)
{
	int c;

	for(c = 1; convient(coul, x, c) == 0; c++)
		;
	coul[x] = c;	/* plus petite coul dispo */
	return c;
}

/* k-coloration, backtracking */
void
colorback(int *coul, int x, int k, int *stop)
{
	int c;

	if(x == N)
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
colexact(int *coul)
{
	int k, stop;

	for(k = stop = 0; k < N && stop == 0; k++){
		memset(coul, 0, N * sizeof(int));
		colorback(coul, 0, k, &stop);
		if(stop == 0)
			printf("Pas de coloration en %d couleurs\n", k);
	}
	return k-1;	/* k-colo, nb chromatique */
}

int
colorglouton(int *coul)
{
	int i, c, nc;

	nc = 0;
	memset(coul, 0, N * sizeof(int));
	for(i = 0; i < N; i++)
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
	for(i = 0; i < N; i++)
		printf("%d\t", dsat[i]);
	printf("\n");
}

int
maxdsat(int *dsat)
{
	int i, s;

	for(s = 0, i = 0; i < N; i++)
		if(dsat[s] < dsat[i] || (dsat[s] == dsat[i] && deg[s] < deg[i]))
			s = i;
	return s;
}

void
actuvois(int *dsat, int *coul)
{
	int i, j, c;

	for(i = 0; i < N; i++){
		/* compte coul voisines à i */
		for(j = 0, c = 0; j < N; j++)
			if(adj[i][j] && coul[j] > 0)
				c++;
		if(c > 0 && dsat[i] > -1)
			dsat[i] = c;	/* reste deg[i] si aucun voisin color */
	}
}

int
dsatur(int *coul)
{
	int s, c, nc, soc, dsat[N];

	memset(coul, 0, N * sizeof(int));
	memmove(dsat, deg, N * sizeof(int));
	for (soc = nc = 0; soc < N; soc++){
		s = maxdsat(dsat);
		if((c = colorsom(coul, s)) > nc)
			nc = c;
		dsat[s] = -1;
		actuvois(dsat, coul);
		prttabdsat(dsat, s, c);
	}
	return nc;
}

int
main()
{
	int i;

	srand((unsigned)time(NULL));
	gengraph(N, 50);
	initdeg();
	affgraph(N);
	printf("nbchroma : %d\n", colexact(coul1));
	for(i = 0; i < N; i++)
		printf("couleur de %d : %d\n", i, coul1[i]);
	colorglouton(coul2);
	for(i = 0; i < N; i++)
		printf("(glout) col %d : %d deg %d\n", i, coul2[i], deg[i]);
	printf("nb coul (dsatur) : %d\n", dsatur(coul3));
	return 0;
}
