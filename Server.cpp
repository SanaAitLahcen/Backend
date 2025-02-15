#include "Server.h"

namespace MyServer {
    Server::Server() 
    {
        setupRoutes();
    }

    void Server::setupRoutes() 
    {
        // D�finir les en-t�tes CORS par d�faut(pour n'est pas bloqu�e les requetes )
        httplib::Headers headers = {
            {"Access-Control-Allow-Origin", "*"},
            {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
            {"Access-Control-Allow-Headers", "Content-Type"}
        };
        svr.set_default_headers(headers);

        // G�rer les requ�tes OPTIONS (pr�flight)
        svr.Options("/bfs", [](const httplib::Request&, httplib::Response& res) 
        {
            res.status = 204; // No Content
        });

        svr.Options("/dijkstra", [](const httplib::Request&, httplib::Response& res) 
        {
            res.status = 204; // No Content
           
        });

        // D�finir les routes
        svr.Post("/bfs", [this](const httplib::Request& req, httplib::Response& res) 
        {
            handleBFS(req, res);
        });

        svr.Post("/dijkstra", [this](const httplib::Request& req, httplib::Response& res) 
        {
            handleDijkstra(req, res);
        });

        // Endpoint pour la page d'accueil
        svr.Get("/", [](const httplib::Request& req, httplib::Response& res) 
        {
            res.set_content("Server is up and running!", "text/plain");
        });
    }

    void Server::handleBFS(const httplib::Request& req, httplib::Response& res)
    {
        try
        {
            // Parsing du JSON
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

            // Ex�cuter BFS et obtenir le chemin et les cases visit�es
            auto result1 = g.BFS(
                static_cast<int>(start[0] * maze[0].size() + start[1]),
                static_cast<int>(end[0] * maze[0].size() + end[1])
            );
            std::vector<int> path = result1.first;
            std::vector<int> visited = result1.second;

            // Convertir le chemin et les cases visit�es en coordonn�es [x, y]
            std::vector<std::vector<int>> pathCoords;
            for (const auto& node : path)
            {
                pathCoords.push_back
                ({
                    //divisant chaque index de n�ud par la largeur du labyrinthe pour obtenir la ligne (x), 
                    // et en prenant le reste de la division pour obtenir la colonne (y).
                    static_cast<int>(node / maze[0].size()),
                    static_cast<int>(node % maze[0].size())
                    });
            }


            std::vector<std::vector<int>> visitedCoords;
            for (const auto& node : visited)
            {
                visitedCoords.push_back
                ({
                    static_cast<int>(node / maze[0].size()),
                    static_cast<int>(node % maze[0].size())
                    });
            }
            // Dans handleBFS et handleDijkstra
            std::cout << "Visited Coords: ";
            for (const auto& coord : visitedCoords) {
                std::cout << "[" << coord[0] << ", " << coord[1] << "] ";
            }
            std::cout << std::endl;
            // Retourner le chemin et les cases visit�es sous forme de JSON
            json response;
            response["path"] = pathCoords;
            response["visited"] = visitedCoords;
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e)
        {
            std::cerr << "Erreur : " << e.what() << std::endl;
            res.status = 500;
            res.set_content("Erreur interne du serveur", "text/plain");
        }
    }



    void Server::handleDijkstra(const httplib::Request& req, httplib::Response& res)
    {
        try
        {
            // Parser le JSON re�u
            auto body = json::parse(req.body);

            // Extraire le labyrinthe, le point de d�part et le point d'arriv�e
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
                            g.addEdge(i * maze[i].size() + j, (i - 1) * maze[i].size() + j, 1);
                        }
                        if (j > 0 && maze[i][j - 1] == 0) {
                            g.addEdge(i * maze[i].size() + j, i * maze[i].size() + (j - 1), 1);
                        }
                    }
                }
            }

            // Ex�cuter Dijkstra et obtenir les distances et les cases visit�es
            auto result = g.shortestPath(static_cast<int>(start[0] * maze[0].size() + start[1]),
                static_cast<int>(end[0] * maze[0].size() + end[1]));

            std::vector<int> distances = result.first;
            std::vector<int> visited = result.second;

            // Convertir les cases visit�es en coordonn�es [x, y]
            std::vector<std::vector<int>> visitedCoords;
            for (const auto& node : visited)
            {
                visitedCoords.push_back
                ({
                    static_cast<int>(node / maze[0].size()),
                    static_cast<int>(node % maze[0].size())
                });
            }
            // Dans handleBFS et handleDijkstra
            std::cout << "Visited Coords: ";
            for (const auto& coord : visitedCoords) 
            {
                std::cout << "[" << coord[0] << ", " << coord[1] << "] ";
            }
            std::cout << std::endl;

            // Retourner les distances et les cases visit�es sous forme de JSON
            json response;
            response["distances"] = distances;
            response["visited"] = visitedCoords;
            res.set_content(response.dump(), "application/json");
        }
        catch (const std::exception& e)
        {
            std::cerr << "Erreur : " << e.what() << std::endl;
            res.status = 500;
            res.set_content("Erreur interne du serveur", "text/plain");
        }
    }

    void Server::start() 
    {
        svr.listen("localhost", 8080);
        std::cout << "Serveur d�marr� sur http://localhost:8080" << std::endl;
    }
}