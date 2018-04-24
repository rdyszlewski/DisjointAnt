#include "Graph.hpp"
#include <functional>

void print(Graph::Edge* x)
{
	std::cout << x->weight;
}

int main()
{
	Graph graph = Graph();
	int data[] = { 0,0,3,0,0,0,0,1,0,0,0,0,0,4,1,0,0,0,0,0,0,0,0,0,0 };
	int source[] = { 1,2 };
	int target[] = { 4,5 };
	graph.Init(data, 5, source, target);
	//auto lambda = [&](Graph::Edge& x) {std::cout << x.weight; };
	std::function<void(Graph::Edge*)> func_obj = &print;
	graph.ForEach(func_obj);
	graph.Release();
	system("pause");
}

