#pragma once
#include "Graph.hpp"
#include "Ant.h"
#include <limits>

class AntColony
{
public:
	AntColony(int steps);
	~AntColony();

	void Start(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta);
private:
	void InitPheromones(Graph* graph);
	std::vector<Ant*> CreateAnts(int numberAnts, int numberColony, Graph* graph);
	void StartAnts();
	void StartAnt(Ant* ant);
	void ChooseBestPath();
	void ResetAnts();

	void ReleaseAnts();
	void Init(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta);

	int CalculateObjectiveFunction();
private:
	std::vector<Ant*> m_ants;
	Graph* m_graph;

	int m_colony_number; //TODO zmieniæ nazwê
	int m_total_ants;

	// PARAMETRY
	int m_step_number;
	int m_ant_number;
	float m_alpha;
	float m_beta;
	float m_p;
	int m_steps;
	int m_current_step;

	//TODO zainicjowaæ to
	std::vector<std::vector<unsigned int>> m_best_path;
	std::vector<Ant*> m_best_ant;
	std::vector<int> m_best_distance;

private:
	const double MIN_PHEROMONE = 0.001;
	const double MAX_PHEROMONE = 0.999;
};

