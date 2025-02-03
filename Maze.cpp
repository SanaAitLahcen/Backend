#include "json.hpp"
#include "Graphe.h"
#include "httplib.h"
#include <iostream>

using json = nlohmann::json;
using namespace httplib;

int main() 
{
    httplib::Server svr;

    // Définir les en-têtes CORS par défaut
    httplib::Headers headers = 
    {
        {"Access-Control-Allow-Origin", "*"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    };
    svr.set_default_headers(headers);

    // Gérer les requêtes OPTIONS (préflight)
    svr.Options("/bfs", [](const httplib::Request&, httplib::Response& res)
        {
        res.status = 204; // No Content
        });

    svr.Options("/dijkstra", [](const httplib::Request&, httplib::Response& res)
        {
        res.status = 204; // No Content
        });

    // Endpoint pour BFS
// Endpoint pour BFS
    svr.Post("/bfs", [](const httplib::Request& req, httplib::Response& res) 
        {
        try
        {
            auto body = json::parse(req.body);
            auto maze = body["maze"].get<std::vector<std::vector<int>>>();
            auto start = body["start"].get<std::vector<int>>();
            auto end = body["end"].get<std::vector<int>>();

            // Convertir le labyrinthe en graphe
            Graphe g(maze.size() * maze[0].size());
            for (size_t i = 0; i < maze.size(); ++i) 
            {
                for (size_t j = 0; j < maze[i].size(); ++j) 
                {
                    if (maze[i][j] == 0) 
                    {
                        if (i > 0 && maze[i - 1][j] == 0) 
                        {
                            g.addEdge(
                                static_cast<int>(i * maze[i].size() + j),
                                static_cast<int>((i - 1) * maze[i].size() + j),
                                1
                            );
                        }
                        if (j > 0 && maze[i][j - 1] == 0) 
                        {
                            g.addEdge(
                                static_cast<int>(i * maze[i].size() + j),
                                static_cast<int>(i * maze[i].size() + (j - 1)),
                                1
                            );
                        }
                    }
                }
            }

            // Exécuter BFS et obtenir le chemin
            auto path = g.BFS(
                static_cast<int>(start[0] * maze[0].size() + start[1]),
                static_cast<int>(end[0] * maze[0].size() + end[1])
            );

            // Convertir le chemin en coordonnées [x, y]
            std::vector<std::vector<int>> pathCoords;
            for (const auto& node : path) 
            {
                pathCoords.push_back(
                    {
                    static_cast<int>(node / maze[0].size()),
                    static_cast<int>(node % maze[0].size())
                    });
            }

            // Retourner le chemin sous forme de JSON
            json response;
            response["path"] = pathCoords; // Ajouter le chemin à la réponse
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) 
        {
            std::cerr << "Erreur : " << e.what() << std::endl;
            res.status = 500;
            res.set_content("Erreur interne du serveur", "text/plain");
        }
        });





    svr.Post("/dijkstra", [](const httplib::Request& req, httplib::Response& res) {
        try {
            // Parser le JSON reçu
            auto body = json::parse(req.body);

            // Extraire le labyrinthe, le point de départ et le point d'arrivée
            auto maze = body["maze"].get<std::vector<std::vector<int>>>();
            auto start = body["start"].get<std::vector<int>>();
            auto end = body["end"].get<std::vector<int>>();

            // Convertir le labyrinthe en graphe
            Graphe g(maze.size() * maze[0].size());
            for (size_t i = 0; i < maze.size(); ++i) 
            {
                for (size_t j = 0; j < maze[i].size(); ++j) 
                {
                    if (maze[i][j] == 0) 
                    {
                        if (i > 0 && maze[i - 1][j] == 0) g.addEdge(i * maze[i].size() + j, (i - 1) * maze[i].size() + j, 1);
                        if (j > 0 && maze[i][j - 1] == 0) g.addEdge(i * maze[i].size() + j, i * maze[i].size() + (j - 1), 1);
                    }
                }
            }

            // Exécuter Dijkstra
            auto distances = g.shortestPath(start[0] * maze[0].size() + start[1]);

            // Retourner les distances sous forme de JSON
            json response;
            response["distances"] = distances;
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e) 
        {
            std::cerr << "Erreur : " << e.what() << std::endl;
            res.status = 500;
            res.set_content("Erreur interne du serveur", "text/plain");
        }
        });


    // Endpoint pour la page d'accueil
    svr.Get("/", [](const Request& req, Response& res) 
        {
        res.set_content("Server is up and running!", "text/plain");
        });

    // Démarrer le serveur
    svr.listen("localhost", 8080);
    std::cout << "Serveur démarré sur http://localhost:8080" << std::endl;
    return 0;
}