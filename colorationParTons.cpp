#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <ctime>
#include <algorithm>

using namespace std;

const int n = 7; // nombre de sommets
int p = 50;
//int adj[n][n];       // matrice d'adjacence du graphe
int adj[7][7] = {
{0, 1, 0, 0, 0, 0, 1},
{1, 0, 1, 0, 0, 1, 0},
{0, 1, 0, 1, 0, 0, 0},
{0, 0, 1, 0, 1, 0, 0},
{0, 0, 0, 1, 0, 1, 0},
{0, 1, 0, 0, 1, 0, 1},
{1, 0, 0, 0, 0, 1, 0}
};

int couleur1[n];     // couleurs des sommets pour l'agorithme exact
int couleur2[n];     // couleus de l'algorithme glouton
int couleur3[n];     // couleur de l'agorithme DSATUR
vector<int> coul3[n];
bool trouve = false; // permet de stopper l'algorithme exact
                     // quand une coloration  a ete trouvee

void generegraphe(int n, int p)
{
    srand(time(NULL));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int chance = rand() % 100;
            if (chance < p)
            {
                adj[i][j] = 1;
                adj[j][i] = 1;
            }
            else
            {
                adj[i][j] = 0;
                adj[j][i] = 0;
            }
        }
    }
}

// algorithme glouton pour la coloration d'un graphe
int colorGlouton()
{
    for (int i = 0; i < n; i++)
        couleur2[i] = -1;

    int nbmaxCouleur = 0;

    for (int i = 0; i < n; i++)
    {
        bool coulUtil[n] = {false}; // suivi des couleurs voisines
        for (int j = 0; j < n; j++)
        {
            if (adj[i][j] && couleur2[j] != -1)
            {
                coulUtil[couleur2[j]] = true;
            }
        }

        // Trouver la plus petites couleur non utilisée
        for (int c = 0; c < n; c++)
        {
            if (!coulUtil[c])
            {
                couleur2[i] = c;
                nbmaxCouleur = max(nbmaxCouleur, c);
                break;
            }
        }
    }

    return nbmaxCouleur + 1;
}

int convientons(int x, int y, int b)
{
    int i, j;

    for(i = 0; i < b; i++){
        for(j = 0; j < b; j++)
            if (adj[x][y] && (coul3[x][i] == coul3[y][j]))
                return false;
    }
    return true;
}

int glouttons(int b)
{
    int a, i, y, x; 

    a = 1;
    for(i = 0; i < n; i++)
        coul3[i].assign(b, 0);

    for(x = 0; x < n; x++){
        for(y = 0; y < n; y++){
            if(convientons(x, y, b)) /* fonction speciale */
                coul3[x].push_back(a);
            else
                a++;
        }
    }


    return a;
}

void afficheGraphe(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << "Sommet : " << i << " : ";
        for (int j = 0; j < n; j++)
        {
            if (adj[i][j])
                cout << j << " ";
        }
        cout << endl;
    }
}

bool convient(int x, int c)
{
    for (int i = 0; i < x; i++)
        if (adj[x][i] && (couleur1[i] == c))
            return false;
    return true;
}

void colorRR(int x, int k)
{
    if (x == n)
    {
        cout << "Coloration en " << k << " couleurs trouvee" << endl;
        for (int i = 0; i < n; i++)
            cout << "couleur de " << i << " : " << couleur1[i] << endl; // int z;cin >> z;
        trouve = true;
    }
    else
        for (int c = 1; c <= k; c++)
            if (convient(x, c))
            {
                couleur1[x] = c;
                colorRR(x + 1, k);
                if (trouve)
                    return;
            }
}

void colorExact(int k)
{
    for (int i = 0; i < n; i++)
        couleur1[i] = 0;
    colorRR(0, k);
    if (!trouve)
        cout << "Pas de coloration en " << k << " couleurs" << endl;
}

/* Le nombre chromatique d’un graphe est le nombre minimal de couleurs nécessaires
 pour colorier ses sommets sans que deux sommets adjacents aient la même couleur.*/

int nbChromatique()
{
    for (int k = 1; k <= n; k++)
    {
        trouve = false;
        colorExact(k);
        if (trouve)
            return k; // on retourne k dès qu'on trouve une solution
    }
    return n; // Par défaut, le pire cas est n couleurs
}

/* DSATUR => Colorie d'abord le sommet avec le plus de voisins
Puis, on choisit à chaque étape le sommet avec le plus de contraintes (Degré de staturation)
On lui donne la plus petite couleur disponible
*/

int DSATUR()
{
    for (int i = 0; i < n; i++)
        couleur3[i] = -1;

    int maxDeg = -1, premierSommet = 0;
    for (int i = 0; i < n; i++)
    {
        int deg = 0;
        for (int j = 0; j < n; j++)
        {
            if (adj[i][j])
                deg++; // calculer le nombre de voison du sommet i
        }

        if (deg > maxDeg)
        { // choisir le premier sommet avec le plus grand nombre de sommet
            maxDeg = deg;
            premierSommet = i;
        }
    }

    couleur3[premierSommet] = 0; // colorer le premier sommet avec 0
    for (int pas = 1; pas < n; pas++)
    {
        int maxDSAT = -1, sommetChoisi = -1;

        for (int i = 0; i < n; i++)
        {
            if (couleur3[i] == -1)
            {
                int DSAT = 0; // Récupérer le deg de saturation du sommet i
                bool coulUtils[n] = {false};

                for (int j = 0; j < n; j++)
                {
                    if (adj[i][j] && couleur3[j] != -1)
                    {
                        coulUtils[couleur3[j]] = true;
                    }
                }

                for (int c = 0; c < n; c++)
                {
                    if (coulUtils[c])
                        DSAT++;
                }

                // pour répérer le sommet avec le plus de contraintes
                if (DSAT > maxDSAT)
                {
                    maxDSAT = DSAT;
                    sommetChoisi = i;
                }
            }
        }

        bool coulUtils[n] = {false};
        for (int j = 0; j < n; j++)
        {
            if (adj[sommetChoisi][j] && couleur3[j] != -1)
            {
                coulUtils[couleur3[j]] = true;
            }
        }

        for (int c = 0; c < n; c++)
        {
            if (!coulUtils[c])
            {
                couleur3[sommetChoisi] = c;
                break;
            }
        }
    }

    return *max_element(couleur3, couleur3 + n) + 1;
}

// méthode de comparaison
/*void simul(int m)
{
    for (int i = 0; i < m; i++)
    {
        generegraphe(n, p);
        afficheGraphe(n);
        // Teste algo glouton
        int coulGl = colorGlouton();
        cout << "Résultat glouton = " << coulGl << " couleurs utilisées\n";

        // Teste algo DSATUR
        int coulDS = DSATUR();
        cout << "Résultat DSATUR = " << coulDS << " couleurs utilisées\n\n";

        if(coulGl > coulDS){
            cout<<"L'algorithme DSATUR rend une meilleure solutions\n\n";
        }else if (coulGl < coulDS){
            // C'est rare
            cout<<"L'algorithme Glouton rend une meilleure solutions\n\n";
        }else{
            cout<<"Les deux algorithmes donnent la même solution\n\n";

        }
    }
}*/

int main()
{
    /*generegraphe(n, p);
    afficheGraphe(n);
    int k = nbChromatique();
    cout << "nombre de chromatiques pour n = " << n << " et p = " << p << " est " << k << endl;

    // Teste algo glouton
    int coulGl = colorGlouton();
    cout << "Résultat glouton = " << coulGl << " couleurs utilisées\n";

    // Teste algo DSATUR
    int coulDS = DSATUR();
    cout << "Résultat DSATUR = " << coulDS << " couleurs utilisées\n\n";*/

    //simul(5);
    //int coulGl = colorGlouton(1);
    //generegraphe(n, p);
    afficheGraphe(n);
    int cougl = glouttons(1);
    printf("%d\n", cougl);
    return 0;
}