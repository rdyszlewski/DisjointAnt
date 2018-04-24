#pragma once
#include <vector>
#include "Graph.hpp"
#include <random>

typedef unsigned int uint;

class Ant
{
public:
	Ant(uint id, uint colony, uint numberInColony,
		uint homeVertex, uint endVertex, uint numberVertices);
	~Ant();

	const uint GetDistance() const;
	void UpdatePheromone(Graph* graph, double factor);
	void LookFor(Graph* graph, unsigned short** bestPaths);
	inline std::vector<uint> GetPath() const
	{
		return m_path;
	}
	void Reset();

	inline uint GetId()
	{
		return m_id;
	}
	inline uint GetColony()
	{
		return m_colony;
	}
private:
	void Step(Graph* graph, unsigned short** bestPaths);
	void Visit(uint vertex);
	uint RandomVertex(std::vector<uint> possibleVertex);
	void GoBack();
	void InitVisitedVertices(uint numberVertices);
	double CalculatePheromoneValue(uint vertex1, uint vertex2, double factor, Graph* graph);
	std::vector<uint>& ExcludePossibleVertex(std::vector<uint>& possibleVertex, unsigned short** bestPaths);
private:
	uint m_id;
	uint m_colony;
	uint m_number_in_colony;
	uint m_home_vertex;
	uint m_end_vertex;
	uint m_current_vertex;
	std::vector<uint> m_path;
	bool* m_visited_vertices;

	uint m_number_vertices;
};

