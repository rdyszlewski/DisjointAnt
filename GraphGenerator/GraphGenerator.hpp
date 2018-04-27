#pragma once
#include "Graph.hpp"

class GraphGenerator
{
public:
	GraphGenerator();
	~GraphGenerator();

	Graph* GenerateGraph(int numberVertices, int numberSources, int weightsRange, int maxVertexLevel);

private:
	int RandomInt(int min, int max);
};

