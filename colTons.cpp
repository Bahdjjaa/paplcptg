#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <cstdlib>
#include <ctime>
#include <algorithm>

/* Coloration par tons :  
    - Chaque sommet reçoit un ensemble de 'b' couleurs parmi 'a' couleurs disponibles
    - pour 2 sommets x et y, il doivent partager moins de d(x,y) couleurs communes
*/

using namespace std;

const int n = 10; // nombre de sommets
int p = 50;
int adj[n][n]; // matrice d'adjacence du graphe

void generegraphe(int n, int p)
{
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (rand() % 100 < p) {
                adj[i][j] = adj[j][i] = 1;
            } else {
                adj[i][j] = adj[j][i] = 0;
            }
        }
        adj[i][i] = 0; // Pas de boucle
    }
}

/* Question 1 : Modifier l'algo glouton*/
// Fonction pour calculer les distance entre deux sommets
int distance (int start, int end){
    if (start == end) return 0;
    vector<int> dist(n, -1);
    queue<int> q;

    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; ++v) {
            if (adj[u][v] && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
                if (v == end) return dist[v];
            }
        }
    }
    return -1; // Pas de chemin
}

// Algo glouton pour la coloration par tons
int colorGloutonTons(int b){
    vector<set<int>> couleursTons(n);
    int max_couleur = 0;
    vector<int> ordre(n);

    for (int i = 0; i < n; ++i) {
        ordre[i] = i;
    }
    random_shuffle(ordre.begin(), ordre.end()); // Ordre aléatoire des sommets

    for (int i_ordre : ordre) {
        set<int> couleurs_sommet;
        for (int c = 0; couleurs_sommet.size() < b; c++) {
            bool valide = true;
            for (int voisin = 0; voisin < n; voisin++) {
                if (i_ordre == voisin) continue;
                int d = distance(i_ordre, voisin);
                if (d > 0 && d != -1) {
                    int couleurs_communes = 0;
                    for (int couleur : couleurs_sommet) {
                        if (couleursTons[voisin].count(couleur)) {
                            couleurs_communes++;
                        }
                    }
                    if (couleursTons[voisin].count(c)) {
                        couleurs_communes++;
                    }
                    if (couleurs_communes >= d) {
                        valide = false;
                        break;
                    }
                }
            }
            if (valide) {
                couleurs_sommet.insert(c);
                max_couleur = max(max_couleur, c + 1);
            }
        }
        couleursTons[i_ordre] = couleurs_sommet;
    }
    return max_couleur;
}

// Algo DSATUR pour la coloration par tons
int colorDSATURTons(int b){
    vector<set<int>> couleursTons(n);
    vector<int> degre(n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            degre[i] += adj[i][j];
        }
    }

    vector<int> saturation(n, 0);
    vector<int> colored(n, -1);
    int num_colored = 0;
    int max_couleur = 0;

    while (num_colored < n)
    {
        int max_sat_node = -1;
        int max_sat_val = -1;

        // trouver le sommet avec la saturation maximale
        for(int i = 0; i < n; i++){
            if(colored[i]==-1){
                if(saturation[i] > max_sat_val){
                    max_sat_val = saturation[i];
                    max_sat_node = i;
                }else if(saturation[i] == max_sat_val && degre[i] > (max_sat_node == -1 ? -1 : degre[max_sat_node])){
                    max_sat_node = i;
                }
            }
        }

        if(max_sat_node == -1) break; // tous les sommets sont colorés

        set<int> couleurs_sommet;
        for(int c = 0; couleurs_sommet.size() < b; c++){
            bool valide = true;
            for(int voisin = 0; voisin <n; voisin++){
                if(max_sat_node == voisin) continue;
                int d = distance(max_sat_node, voisin);
                if(d > 0 && d != -1){
                    int couleurs_communes = 0; 
                    for(int couleur : couleurs_sommet){
                        if(couleursTons[voisin].count(couleur)){
                            couleurs_communes++;
                        }
                    }
                    if(couleursTons[voisin].count(c)){
                        couleurs_communes++;
                    }
                    if(couleurs_communes >= d){
                        valide = false;
                        break;
                    }
                }
            }
            if(valide){
                couleurs_sommet.insert(c);
                max_couleur = max(max_couleur, c+1);
            }
        }
        couleursTons[max_sat_node] = couleurs_sommet;
        colored[max_sat_node] = 1;
        num_colored++; 

        // mettre à jour la saturation des voisins non colorés
        for(int voisin = 0; voisin < n; voisin++){
            if(adj[max_sat_node][voisin] && colored[voisin] == -1){
                set<int> voisin_colors = couleursTons[voisin];
                int sat = 0; 
                for(int color :  couleursTons[max_sat_node]){
                    if(voisin_colors.count(color)){
                        sat++;
                    }
                }
                saturation[voisin] = sat;
            }
        }
    }
    return max_couleur;
}

// Méthode pour tester sur des graphes aléatoires
void testGloutonTons(){ 
    int b_val[] = {1, 2, 3};
    cout<<"== Algo Glouton =="<<endl;
    cout<<"n\tb\ta (couleurs)"<<endl;

    for(int b : b_val) {
        generegraphe(n, p);
        int a = colorGloutonTons(b);
        cout<<n<<"\t"<<b<<"\t"<<a<<endl;
    }
}

// Méthpde pour tester DSATUR sur des graphes aléatoires
void testDSATURTons(){
    int b_val[] = {1, 2, 3};
    cout<<"== Algo DSATUR =="<<endl;
    cout<<"n\tb\ta (couleurs)"<<endl;

    for(int b : b_val) {
        generegraphe(n, p);
        int a = colorDSATURTons(b);
        cout<<n<<"\t"<<b<<"\t"<<a<<endl;
    }
}

int main(){
    testGloutonTons();
    testDSATURTons();
    return 0; 
}