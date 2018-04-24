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

	inline int GetSource(int sourceNumber) 
	{
		if (sourceNumber >= 0 && sourceNumber < m_pairs_number) {
			return m_sources[sourceNumber];
		}
	}
	inline int GetTarget(int targetNumber)
	{
		if (targetNumber >= 0 && targetNumber < m_pairs_number) {
			return m_targets[targetNumber];
		}
	}
	inline int GetWeight(int firstVertex, int secondVertex)
	{
		if (m_data[firstVertex].count(secondVertex)) {
			return m_data[firstVertex].at(secondVertex)->weight;
		}
		return 0;
	}
	inline int GetPheromone(int firstVertex, int secondVertex, int colony)
	{
		if (m_data[firstVertex].count(secondVertex)) {
			return m_data[firstVertex].at(secondVertex)->pheromon[colony];
		}
		return 0;
	}
	inline int GetPairsNumber()
	{
		return m_pairs_number;
	}
	inline int GetVerticesNumber()
	{
		return m_vertices_number;
	}
	std::vector<unsigned int> GetPossibleVertex(int vertex);

	inline void SetPheromone(int firstVertex, int secondVertex, int colony, double value)
	{
		m_data[firstVertex].at(secondVertex)->pheromon[colony] = value;
	}
	inline void AddPheromone(int firstVertex, int secondVertex, int colony, double value)
	{
		m_data[firstVertex].at(secondVertex)->pheromon[colony] += value;
	}
	void ResetPheromones(double value);

private:
	std::map<int, Edge*>* m_data;
	//std::vector<std::map<int, Edge>> m_data;
	int* m_sources;
	int* m_targets;

	int m_pairs_number;
	int m_vertices_number;
	
};

