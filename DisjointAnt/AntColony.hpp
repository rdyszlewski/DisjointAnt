#pragma once
#include "Graph.hpp"
#include "Ant.h"
#include <limits>
#include "WriteListener.hpp"

class AntColony 
{
public:
	AntColony(int steps);
	~AntColony();

	void Start(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta);
	inline void SetWriteListener(WriteListener* writeListener);
private:
	void InitPheromones(Graph* graph);
	std::vector<Ant*> CreateAnts(int numberAnts, int numberColony, Graph* graph);
	void StartAnts();
	void StartAnt(Ant* ant);
	void ChooseBestPaths();
	void ResetAnts();

	void ReleaseAnts();
	void Init(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta);

	int CalculateObjectiveFunction();
	void UpdateBestPaths();
	void ZeroBestPaths();

	void EvaporatePheromone(double factor);
	void FixPheromoneValue();

	void SaveResult(int iteration, std::vector<int>& iterationResults);
private:
	// TODO zmieniæ to wektor wektorów mrówek. Pomo¿e to wypuszczaæ mrówki mrowiskami
	std::vector<Ant*> m_ants;
	Graph* m_graph;

	int m_number_colonies; //TODO zmieniæ nazwê
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
	short** m_best_paths_matrix;
	std::vector<std::vector<unsigned int>> m_best_path;
	std::vector<Ant*> m_best_ant;
	std::vector<int> m_best_distance;

	WriteListener* m_write_listener;

private:
	const double MIN_PHEROMONE = 0.001;
	const double MAX_PHEROMONE = 0.999;
};

