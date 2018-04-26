#pragma once
#include "Graph.hpp"
#include "Ant.h"
#include <limits>

class WriteListener;

class AntColony 
{
public:
	AntColony(int steps);
	~AntColony();

	void Start(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta);
	inline void SetWriteListener(WriteListener* writeListener)
	{
		m_write_listener = writeListener;
	}
private:
	void InitPheromones(Graph* graph);
	std::vector<std::vector<Ant*>> CreateAnts(int numberAnts, int numberColony, Graph* graph);
	void StartAnts(int colony);
	void StartAnt(Ant* ant);
	void ChooseBestPath(int colony);
	void ResetAnts();

	void ReleaseAnts();
	void Init(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta);

	int CalculateObjectiveFunction();
	void UpdateBestPath(int colony);
	void ZeroBestPath(int colony);

	void EvaporatePheromone(double factor);
	void FixPheromoneValue();

	void SaveResult(int iteration, int objectiveFunction);
private:
	// TODO zmieniæ to wektor wektorów mrówek. Pomo¿e to wypuszczaæ mrówki mrowiskami
	std::vector<std::vector<Ant*>> m_ants;
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

	short** m_best_paths_matrix;
	std::vector<std::vector<unsigned int>> m_best_path;
	std::vector<Ant*> m_best_ant;
	std::vector<int> m_best_distance;

	WriteListener* m_write_listener;

	// zmienne pomagaj¹ce wykryæ zmianê funkcji celu w kolenych iteracjach
	double m_last_objective_function;
	int m_same_counter;

private:
	const double MIN_PHEROMONE = 0.001;
	const double MAX_PHEROMONE = 0.999;

	const int MAX_SAME_COUNTER = 5;
};

