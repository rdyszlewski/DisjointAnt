#pragma once
#include <map>
#include <vector>
#include <iostream>

class Graph
{
public:
	struct Edge {
		const int firstNode;
		const int secondNode;
		const int weight;
		float* pheromon;

		Edge(int firstNode, int secondNode, int weight,int numberPheromonTypes):firstNode(firstNode), secondNode(secondNode), weight(weight)
		{
			pheromon = new float[numberPheromonTypes];
		}
	};
	Graph();
	~Graph();

	void Init(int* data, int numberVertices, int* sources, int* targets);
	void Release();

	inline int GetSource(int sourceNumber);
	inline int GetTarget(int targetNumber);
	inline int GetWeight(int firstVertex, int secondVertex);
	inline int GetPheromone(int firstVertex, int secondVertex, int colonyNumber);
	inline int GetPairsNumber();
	inline int GetVerticesNumber();
	std::vector<unsigned int> GetPossibleVertex(int vertex);

	inline void SetPheromone(int firstVertex, int secondVertex, int colony, double value);
	inline void AddPheromone(int firstVertex, int secondVertex, int colony, double value);
	void ResetPheromones(double value);

private:
	std::map<int, Edge*>* m_data;
	//std::vector<std::map<int, Edge>> m_data;
	int* m_sources;
	int* m_targets;

	int m_pairs_number;
	int m_vertices_number;
	
};

