#include "Graphe.h"
#include <set>

//// Constructeur du graphe
Graphe::Graphe(int V) 
{
    this->V = V;                           // Initialisation du nombre de sommets
    adj = new list<pair<int, int>>[V];     // Allocation de la liste d'adjacence
}

// Méthode pour ajouter une arête au graphe
void Graphe::addEdge(int u, int v, int w) 
{
    adj[u].push_back(make_pair(v, w));     // Ajout de l'arête u -> v avec poids w
    adj[v].push_back(make_pair(u, w));     // Ajout de l'arête v -> u pour un graphe non orienté
}

// Destructeur pour libérer la mémoire
Graphe::~Graphe() 
{
    delete[] adj;                          // Libérer la mémoire allouée pour la liste d'adjacence
}



std::pair<std::vector<int>, std::vector<int>> Graphe::BFS(int start, int end) {
    std::vector<int> dist(V, -1); // Distance depuis start, -1 signifie inatteignable
    std::vector<int> parent(V, -1); // Pour enregistrer le parent de chaque nœud
    std::vector<int> visited; // Pour enregistrer les cases visitées
    std::queue<int> q; // File pour le parcours en largeur

    dist[start] = 0; // Distance au sommet de départ est 0
    q.push(start); // Ajouter le sommet de départ à la file
    visited.push_back(start); // Enregistrer le sommet de départ comme visité

    while (!q.empty()) {
        int u = q.front(); // Récupérer le sommet en tête de file
        q.pop();

        // Si on atteint le nœud d'arrivée, reconstruire le chemin
        if (u == end)
        {
            std::vector<int> path;
            for (int node = end; node != -1; node = parent[node]) 
            {
                path.push_back(node);
            }
            std::reverse(path.begin(), path.end()); // Inverser pour obtenir l'ordre correct
            return { path, visited }; // Retourner le chemin et les cases visitées
        }

        // Parcourir les voisins du sommet courant
        for (auto& voisin : adj[u]) {
            int v = voisin.first; // Sommet voisin

            if (dist[v] == -1) { // Si le voisin n'a pas encore été visité
                dist[v] = dist[u] + 1; // Distance = distance du parent + 1
                parent[v] = u; // Enregistrer le parent du voisin
                q.push(v); // Ajouter le voisin à la file
                visited.push_back(v); // Enregistrer le voisin comme visité
            }
        }
    }

    // Si aucun chemin n'est trouvé, retourner un vecteur vide
    return { {}, visited };
}




std::pair<std::vector<int>, std::vector<int>> Graphe::shortestPath(int src, int end) {
    std::vector<int> dist(V, INT_MAX); // Distance minimale de src à chaque sommet
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;
    std::vector<int> visitedNodes; // Pour enregistrer les nœuds visités

    dist[src] = 0; // La distance au sommet source est 0
    pq.push({ 0, src }); // Ajouter la source à la file de priorité

    while (!pq.empty()) {
        int u = pq.top().second; // Récupérer le sommet avec la plus petite distance
        pq.pop();

        // Si on atteint le point d'arrivée, arrêter l'algorithme
        if (u == end) {
            break;
        }

        visitedNodes.push_back(u); // Enregistrer le nœud visité

        // Parcourir les voisins du sommet courant
        for (auto& voisin : adj[u]) {
            int v = voisin.first; // Sommet voisin
            int poids = voisin.second; // Poids de l'arête u -> v

            // Relâchement : Vérifier si une distance plus courte est trouvée
            if (dist[u] + poids < dist[v]) {
                dist[v] = dist[u] + poids;
                pq.push({ dist[v], v });
            }
        }
    }

    return { dist, visitedNodes };
}