#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <climits>

using namespace std;
#define INF 0x3f3f3f3f

class Graphe 
{
    int V;
    list<pair<int, int>>* adj;

public:
   Graphe(int);
   ~Graphe();
   void addEdge(int u, int v, int w);
   std::pair<std::vector<int>, std::vector<int>> BFS(int start, int end);
   pair<std::vector<int>, std::vector<int>> shortestPath(int src, int end); // Algorithme de Dijkstra
};



