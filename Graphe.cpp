#include "Graphe.h"
#include <set>

//// Constructeur du graphe
Graphe::Graphe(int V) 
{
    this->V = V;                           // Initialisation du nombre de sommets
    adj = new list<pair<int, int>>[V];     // Allocation de la liste d'adjacence
}

// M�thode pour ajouter une ar�te au graphe
void Graphe::addEdge(int u, int v, int w) 
{
    adj[u].push_back(make_pair(v, w));     // Ajout de l'ar�te u -> v avec poids w
    adj[v].push_back(make_pair(u, w));     // Ajout de l'ar�te v -> u pour un graphe non orient�
}

// Destructeur pour lib�rer la m�moire
Graphe::~Graphe() 
{
    delete[] adj;                          // Lib�rer la m�moire allou�e pour la liste d'adjacence
}



std::pair<std::vector<int>, std::vector<int>> Graphe::BFS(int start, int end) {
    std::vector<int> dist(V, -1); // Distance depuis start, -1 signifie inatteignable
    std::vector<int> parent(V, -1); // Pour enregistrer le parent de chaque n�ud
    std::vector<int> visited; // Pour enregistrer les cases visit�es
    std::queue<int> q; // File pour le parcours en largeur

    dist[start] = 0; // Distance au sommet de d�part est 0
    q.push(start); // Ajouter le sommet de d�part � la file
    visited.push_back(start); // Enregistrer le sommet de d�part comme visit�

    while (!q.empty()) {
        int u = q.front(); // R�cup�rer le sommet en t�te de file
        q.pop();

        // Si on atteint le n�ud d'arriv�e, reconstruire le chemin
        if (u == end)
        {
            std::vector<int> path;
            for (int node = end; node != -1; node = parent[node]) 
            {
                path.push_back(node);
            }
            std::reverse(path.begin(), path.end()); // Inverser pour obtenir l'ordre correct
            return { path, visited }; // Retourner le chemin et les cases visit�es
        }

        // Parcourir les voisins du sommet courant
        for (auto& voisin : adj[u]) {
            int v = voisin.first; // Sommet voisin

            if (dist[v] == -1) { // Si le voisin n'a pas encore �t� visit�
                dist[v] = dist[u] + 1; // Distance = distance du parent + 1
                parent[v] = u; // Enregistrer le parent du voisin
                q.push(v); // Ajouter le voisin � la file
                visited.push_back(v); // Enregistrer le voisin comme visit�
            }
        }
    }

    // Si aucun chemin n'est trouv�, retourner un vecteur vide
    return { {}, visited };
}




std::pair<std::vector<int>, std::vector<int>> Graphe::shortestPath(int src, int end) {
    std::vector<int> dist(V, INT_MAX); // Distance minimale de src � chaque sommet
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    std::vector<int> visitedNodes; // Pour enregistrer les n�uds visit�s

    dist[src] = 0; // La distance au sommet source est 0
    pq.push({ 0, src }); // Ajouter la source � la file de priorit�

    while (!pq.empty()) {
        int u = pq.top().second; // R�cup�rer le sommet avec la plus petite distance
        pq.pop();

        // Si on atteint le point d'arriv�e, arr�ter l'algorithme
        if (u == end) {
            break;
        }

        visitedNodes.push_back(u); // Enregistrer le n�ud visit�

        // Parcourir les voisins du sommet courant
        for (auto& voisin : adj[u]) {
            int v = voisin.first; // Sommet voisin
            int poids = voisin.second; // Poids de l'ar�te u -> v

            // Rel�chement : V�rifier si une distance plus courte est trouv�e
            if (dist[u] + poids < dist[v]) {
                dist[v] = dist[u] + poids;
                pq.push({ dist[v], v });
            }
        }
    }

    return { dist, visitedNodes };
}