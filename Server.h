#pragma once

#include "json.hpp"
#include "Graphe.h"
#include "httplib.h"
#include <iostream>

using json = nlohmann::json;


namespace MyServer 
{

    class Server 
    {
    public:
        Server();
        void start();

    private:
        void setupRoutes();
        void handleBFS(const httplib::Request& req, httplib::Response& res);
        void handleDijkstra(const httplib::Request& req, httplib::Response& res);

        httplib::Server svr;
    };

} 

