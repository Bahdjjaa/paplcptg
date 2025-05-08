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
int trouv = 0;	/* stoppe algo exact quand coloration trouvee */

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
	for(i = 0; i < x; i++)
		if(adj[x][i] && (coul[i] == c))
			return 0;
	return 1;
}

void
colorrr(int x, int k)
{
	int i, c;

	if(x == N){
		printf("Coloration en %d couleurs trouvée.\n", k);
		for(i = 0; i < N; i++)
			printf("couleur de %d : %d\n", i, coul1[i]);
		trouv = 1;
	}else{
		for(c = 1; c <= k; c++){
			if(convient(coul1, x, c)){
				coul1[x] = c;
				colorrr(x+1, k);
	 			if(trouv)
					return;
			}
		}
	}
}

/* teste si un graphe admet k-coloration */
void
colorexact(int k)
{
	memset(coul1, 0, N * sizeof(int));
	colorrr(0, k);
	if(!trouv)
		printf("Pas de coloration en %d couleurs\n", k);
}

int
nbchroma()
{
	int i;

	/* variation de k */
	for(i = 0; i < N && !trouv; i++){
		colorexact(i);
	}
	return i-1;
}

int
colorglouton()
{
	int i, c, nc;

	nc = 0;
	memset(coul2, 0, N * sizeof(int));
	for(i = 0; i < N; i++){
		for(c = 1; convient(coul2, i, c) == 0; c++)
			;
		coul2[i] = c;	/* plus petite coul dispo */
		nc = (nc < c)? c : nc;
	}
	return nc;
}


int
dsatur(int *coul)
{
	int i, s, c, nc, soc, dsat[N];

	memset(coul, 0, N * sizeof(int));
	memmove(dsat, deg, N * sizeof(int));
	for (soc = nc = 0; soc < N; soc++){
		/* recup dsat max et resolv conflit */
		for(s = 0, i = 0; i < N; i++){
			if(dsat[s] < dsat[i] && dsat[i] > -1)
				s = i;
			else if(dsat[s] == dsat[i])
				s = (deg[s] < deg[i])? i : s;
		}
		/* coloration du sommet s */
		for(c = 1; convient(coul, s, c) == 0; c++)
			;
		coul[s] = c;	/* plus petite coul dispo */
		dsat[s] = -1;
		nc = (nc < c)? c : nc;
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
	printf("nbchroma : %d\n", nbchroma());
	colorglouton();
	for(i = 0; i < N; i++)
		printf("(glout) col %d : %d deg %d\n", i, coul2[i], deg[i]);
	printf("nb coul (dsatur) : %d\n", dsatur(coul3));
	for(i = 0; i < N; i++)
		printf("(dsat) col %d : %d\n", i, coul3[i]);
	return 0;
}
