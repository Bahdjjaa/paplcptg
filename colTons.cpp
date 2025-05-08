#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <chrono>

/* Coloration par tons :  
    - Chaque sommet reçoit un ensemble de 'b' couleurs parmi 'a' couleurs disponibles
    - pour 2 sommets x et y, il doivent partager moins de d(x,y) couleurs communes
*/

using namespace std;

int n_global; // nombre de sommets
int p = 50;
vector<vector<int>> adj; // matrice d'adjacence du graphe
/*int adj[7][7] = {
    {0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 1, 0},
    {0, 1, 0, 1, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 0},
    {0, 0, 0, 1, 0, 1, 0},
    {0, 1, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 0, 1, 0}
    };
*/

// méthode pour redimensionner la matrice
void redimMatrice(int n){
    n_global = n;
    adj.assign(n, vector<int>(n, 0));
}

void generegraphe(int n, int p)
{
    redimMatrice(n);
    srand(time(NULL));
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (rand() % 100 < p) {
                adj[i][j] = adj[j][i] = 1;
            }
        }
        adj[i][i] = 0; // Pas de boucle
    }
}

void generergrapheCycle(int n){
    redimMatrice(n);
    // Ajouter les arrêtes du cycle
    for(int i = 0; i < n; i++){
        int voisin1 = (i+1)%n; // modulo n pour boucler
        adj[i][voisin1] = adj[voisin1][i] = 1;
    }
}

/* Question 1 : Modifier l'algo glouton*/
// Fonction pour calculer les distance entre deux sommets
int distance (int start, int end){
    if (start == end) return 0;
    vector<int> dist(n_global, -1);
    queue<int> q;

    dist[start] = 0;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n_global; ++v) {
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
    vector<set<int>> couleursTons(n_global);
    int max_couleur = 0;
    vector<int> ordre(n_global);

    for (int i = 0; i < n_global; i++) {
        ordre[i] = i;
    }
    random_shuffle(ordre.begin(), ordre.end()); // Ordre aléatoire des sommets

    //cout<<"Glouton - couleurs par sommet (b = "<<b<<") ==" << endl;
    for (int i_ordre : ordre) {
        set<int> couleurs_sommet;
        for (int c = 1; ; c++) {
            if(couleurs_sommet.size() == b) break;
            bool valide = true;
            for (int voisin = 0; voisin < n_global; voisin++) {
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
        /*cout<<"Sommet "<<i_ordre<<": {";
        int count = 0; 
        for(int coul : couleurs_sommet){
            cout<<coul;
            if(count <couleurs_sommet.size() - 1)
                cout<<", ";
            count++;
        }
        cout<<"}"<<endl; */
    }
    return max_couleur;
}

// Algo DSATUR pour la coloration par tons
int colorDSATURTons(int b){
    vector<set<int>> couleursTons(n_global);
    vector<int> degre(n_global);
    for(int i = 0; i < n_global; i++){
        for(int j = 0; j < n_global; j++){
            degre[i] += adj[i][j];
        }
    }

    vector<int> saturation(n_global, 0);
    vector<int> colored(n_global, -1);
    int num_colored = 0;
    int max_couleur = 0;

    //cout<<"DSATUR - Couleurs oar sommet (b = "<<b<<") == "<<endl;
    while (num_colored < n_global)
    {
        int max_sat_node = -1;
        int max_sat_val = -1;

        // trouver le sommet avec la saturation maximale
        for(int i = 0; i < n_global; i++){
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
        for(int c = 1; ; c++){
            if(couleurs_sommet.size() == b) break;
            bool valide = true;
            for(int voisin = 0; voisin <n_global; voisin++){
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
        /*cout<<"Sommet "<< max_sat_node<<": {";
        int count = 0;
        for(int coul : couleurs_sommet){
            cout<<coul;
            if(count < couleurs_sommet.size() -1)
                cout<<", ";
            count++;
        }
        cout<<"}"<<endl;*/

        // mettre à jour la saturation des voisins non colorés
        for(int voisin = 0; voisin < n_global; voisin++){
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
/*
// Méthode pour tester sur des graphes aléatoires
void testGloutonTons(){ 
    int b_val[] = {1, 2, 3};
    cout<<"== Algo Glouton =="<<endl;
    cout<<"n\tb\ta (couleurs)"<<endl;

    for(int b : b_val) {
        //generegraphe(n, p);
        //generergrapheCycle(n);
        int a = colorGloutonTons(b);
        cout<<n_global<<"\t"<<b<<"\t"<<a<<endl;
    }
}

// Méthpde pour tester DSATUR sur des graphes aléatoires
void testDSATURTons(){
    int b_val[] = {1, 2, 3};
    cout<<"== Algo DSATUR =="<<endl;
    cout<<"n\tb\ta (couleurs)"<<endl;

    for(int b : b_val) {
        //generegraphe(n, p);
        //generergrapheCycle(n);
        int a = colorDSATURTons(b);
        cout<<n_global<<"\t"<<b<<"\t"<<a<<endl;
    }
}*/

// tester les algo sur le même graphe
void testAlgos(int n, int b){
    cout<<"~~~ Pour n = "<<n<<", b = "<<b<<" ~~~"<<endl;
    generegraphe(n,p);

    // Algo glouton
    auto debut_glouton = chrono::high_resolution_clock::now();
    int couleurs_glouton = colorGloutonTons(b);
    auto fin_glouton = chrono::high_resolution_clock::now();
    auto duration_glouton = chrono::duration_cast<chrono::milliseconds>(fin_glouton - debut_glouton);
    cout<<"Algo Glouton :  Temps = "<<duration_glouton.count()<< " ms, Couleurs = "<<couleurs_glouton<<endl;

    // Algo DSATUR
    auto debut_dsatur = chrono::high_resolution_clock::now();
    int couleurs_dsatur = colorDSATURTons(b);
    auto fin_dsatur =  chrono::high_resolution_clock::now();
    auto duration_dsatur = chrono::duration_cast<chrono::milliseconds>(fin_dsatur - debut_dsatur);
    cout<<"Algo DSATUR : Temps = "<<duration_dsatur.count()<<" ms, Couleurs = "<<couleurs_dsatur<<endl;

    cout<<endl;

    cout << "--- Tests sur des graphes cycles ---" << endl;
    cout << "--- Cycle C" << n << ", b = " << b << " ---" << endl;
    generergrapheCycle(n);

    // Tester l'algorithme glouton sur le cycle
    auto start_glouton_cycle = chrono::high_resolution_clock::now();
    int couleurs_glouton_cycle = colorGloutonTons(b);
    auto end_glouton_cycle = chrono::high_resolution_clock::now();
    auto duration_glouton_cycle = chrono::duration_cast<chrono::milliseconds>(end_glouton_cycle - start_glouton_cycle);
    cout << "Algo Glouton (Cycle): Temps = " << duration_glouton_cycle.count() << " ms, Couleurs = " << couleurs_glouton_cycle << endl;

    // Tester l'algorithme DSATUR sur le cycle
    auto start_dsatur_cycle = chrono::high_resolution_clock::now();
    int couleurs_dsatur_cycle = colorDSATURTons(b);
    auto end_dsatur_cycle = chrono::high_resolution_clock::now();
    auto duration_dsatur_cycle = chrono::duration_cast<chrono::milliseconds>(end_dsatur_cycle - start_dsatur_cycle);
    cout << "Algo DSATUR (Cycle): Temps = " << duration_dsatur_cycle.count() << " ms, Couleurs = " << couleurs_dsatur_cycle << endl;

    cout << endl;
}

int main(){
    //testGloutonTons();
    //testDSATURTons();
    
    vector<int> n_vals = {10, 20, 50, 100, 200};
    vector<int> b_vals = {1, 2, 3};

    for(int n :  n_vals){
        for(int b : b_vals){
            testAlgos(n, b);
        }
    }


    return 0; 
}