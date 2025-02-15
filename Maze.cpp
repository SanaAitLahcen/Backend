#include "json.hpp"
#include "Graphe.h"
#include "httplib.h"
#include <iostream>


#include "server.h"

int main() {
    // Utiliser le namespace MyServer pour accéder à la classe Server
    MyServer::Server server;
    server.start();
    return 0;
}