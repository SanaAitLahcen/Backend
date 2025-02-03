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








std::vector<int> Graphe::BFS(int start, int end) {
    std::vector<int> dist(V, -1); // Distance depuis start, -1 signifie inatteignable
    std::vector<int> parent(V, -1); // Pour enregistrer le parent de chaque n�ud
    std::queue<int> q; // File pour le parcours en largeur

    dist[start] = 0; // Distance au sommet de d�part est 0
    q.push(start); // Ajouter le sommet de d�part � la file

    std::cout << "D�but du parcours BFS � partir du sommet " << start << std::endl;

    while (!q.empty()) {
        int u = q.front(); // R�cup�rer le sommet en t�te de file
        q.pop();

        std::cout << "Sommet en cours : " << u << " (distance : " << dist[u] << ")" << std::endl;

        // Si on atteint le n�ud d'arriv�e, reconstruire le chemin
        if (u == end) {
            std::vector<int> path;
            for (int node = end; node != -1; node = parent[node]) {
                path.push_back(node);
            }
            std::reverse(path.begin(), path.end()); // Inverser pour obtenir l'ordre correct
            return path;
        }

        // Parcourir les voisins du sommet courant
        for (auto& voisin : adj[u]) {
            int v = voisin.first; // Sommet voisin

            if (dist[v] == -1) { // Si le voisin n'a pas encore �t� visit�
                dist[v] = dist[u] + 1; // Distance = distance du parent + 1
                parent[v] = u; // Enregistrer le parent du voisin
                q.push(v); // Ajouter le voisin � la file

                std::cout << "  -> Visite du voisin " << v << " (nouvelle distance : " << dist[v] << ")" << std::endl;
            }
        }
    }

    std::cout << "Fin du parcours BFS." << std::endl;

    // Si aucun chemin n'est trouv�, retourner un vecteur vide
    return {};
}



std::vector<int> Graphe::shortestPath(int src)
{
    std::vector<int> dist(V, INT_MAX); // Distance minimale de src � chaque sommet
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq; // File de priorit�

    dist[src] = 0; // La distance au sommet source est 0
    pq.push({ 0, src }); // Ajouter la source � la file de priorit�

    std::cout << "D�but de Dijkstra � partir du sommet " << src << std::endl;

    while (!pq.empty())
    {
        int u = pq.top().second; // R�cup�rer le sommet avec la plus petite distance
        pq.pop();

        std::cout << "Sommet en cours : " << u << " (distance : " << dist[u] << ")" << std::endl;

        // Parcourir les voisins du sommet courant
        for (auto& voisin : adj[u])
        {
            int v = voisin.first; // Sommet voisin
            int poids = voisin.second; // Poids de l'ar�te u -> v

            // Rel�chement : V�rifier si une distance plus courte est trouv�e
            if (dist[u] + poids < dist[v])
            {
                dist[v] = dist[u] + poids;
                pq.push({ dist[v], v });

                std::cout << "  -> Mise � jour de la distance du sommet " << v << " : " << dist[v] << std::endl;
            }
        }
    }

    std::cout << "Fin du parcours Dijkstra." << std::endl;

    return dist;
}