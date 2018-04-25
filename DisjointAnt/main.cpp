#include "Graph.hpp"
#include "AntColony.hpp"

void print(Graph::Edge* x)
{
	std::cout << x->weight;
}

int main()
{
	Graph graph = Graph();
	int data[] = { 0,0,3,0,0,0,0,1,0,0,0,0,0,4,1,0,0,0,0,0,0,0,0,0,0 };
	int source[] = { 0,1 };
	int target[] = { 3,4 };
	graph.Init(data, 5, source, target, 2);
	AntColony antColony(10);
	antColony.Start(&graph, 10, 1, 1, 0.1, 1);
	
	graph.Release();
	system("pause");
}

