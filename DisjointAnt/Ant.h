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
	void LookFor(Graph* graph);
	inline std::vector<uint> GetPath() const;
	void Reset();
private:
	void Step(Graph* graph);
	void Visit(uint vertex);
	uint RandomVertex(std::vector<uint> possibleVertex);
	void GoBack();
	void InitVisitedVertices(uint numberVertices);
	double CalculatePheromoneValue(uint vertex1, uint vertex2, double factor, Graph* graph);
	std::vector<uint>& ExcludePossibleVertex(std::vector<uint>& possibleVertex);
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

