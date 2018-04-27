#include "GraphGenerator.hpp"
#include <random>
#include <map>

GraphGenerator::GraphGenerator()
{
}


GraphGenerator::~GraphGenerator()
{
}

Graph* GraphGenerator::GenerateGraph(int numberVertices, int numberSources, int weigthsRange, int maxVertexLevel)
{
	int** adjacencyMatrix = new int*[numberVertices];
	for (int i = 0; i < numberVertices; i++)
	{
		adjacencyMatrix[i] = new int[numberVertices];
		for (int j = 0; j < numberVertices; j++)
		{
			adjacencyMatrix[i][j] = 0;
		}
	}
	//przypisywanie wag
	for (int i = 0; i < numberVertices - numberSources; i++)
	{
		int vertexLevel = RandomInt(numberSources, maxVertexLevel);
		// mapa wierzcho³ków, z którym bêdzie po³¹czony obecny wierzcho³ek
		std::map<int, bool> adjacencyMap;
		for (int j = 0; j < vertexLevel; j++)
		{
			bool insert = false;
			while (!insert)
			{
				int vertex = RandomInt(numberSources, numberVertices - 1);
				if (adjacencyMap.count(vertex)==0) {
					adjacencyMap[vertex] = true;
					int weight = RandomInt(1, weigthsRange);
					adjacencyMatrix[i][vertex] = weight;
					insert = true;
				}
			}
		}
	}
	for (int i = 0; i < numberVertices; i++)
	{
		for (int j = 0; j < numberVertices; j++)
		{
			std::cout << adjacencyMatrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	Graph* graph = new Graph();
	int* sources = new int[numberSources];
	int* targets = new int[numberSources];
	for (int i = 0; i < numberSources; i++)
	{
		sources[i] = i;
		targets[i] = numberSources - i - 1;
	}
	graph->Init(adjacencyMatrix, numberVertices, sources, targets, numberSources);
	return graph;
}

int GraphGenerator::RandomInt(int min, int max)
{
	return min + (std::rand() % (max - min + 1));
}