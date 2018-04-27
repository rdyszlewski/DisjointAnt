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
	inline void SetSameCounter(int counter) {
		m_same_counter = counter;
	}
	inline void SetNumberIteration(int iteration)
	{
		m_steps = iteration;
	}

	inline std::vector<std::vector<unsigned int>>& GetBestPaths()
	{
		return m_best_paths_results;
	}

	inline std::vector<int>& GetBestDistances()
	{
		return m_best_distances;
	}
private:

	struct ObjectiveResult
	{
		int numberPaths;
		int distance;
	};

	void InitPheromones(Graph* graph);
	std::vector<std::vector<Ant*>> CreateAnts(int numberAnts, int numberColony, Graph* graph);
	void StartAnts(int colony);
	void StartAnt(Ant* ant);
	void ChooseBestPathInIteration(int colony);
	void ResetAnts();

	void ReleaseAnts();
	void Init(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta);

	int CalculateObjectiveFunction();
	void UpdateBestPath(int colony);
	void ZeroBestPath(int colony);

	void EvaporatePheromone(double factor);
	void FixPheromoneValue();

	void SaveResult(int iteration, int objectiveFunction);
	void ServeSameObjectiveFunction(int objectiveFunction);
	void SummarizeIteration(int iteration);

	void InitColoniesOrders(int numberColonies);
	void RandomColoniesOrders();
	void InitLastPathsMatrix();
	void InitBestResults(int numberColonies);

	bool CompareObjectiveFunction(ObjectiveResult result1, ObjectiveResult result2);
	void CopyBestPathsResults(std::vector<std::vector<unsigned int>>& result, std::vector<std::vector<unsigned int>>& source);
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
	int m_max_same_counter;
	int m_current_step;

	std::vector<int> m_orders;

	short** m_last_paths_matrix;
	std::vector<std::vector<unsigned int>> m_best_paths;
	std::vector<Ant*> m_best_ants;
	std::vector<int> m_best_distances;

	std::vector<std::vector<unsigned int>> m_best_paths_results;

	WriteListener* m_write_listener;

	// zmienne pomagaj¹ce wykryæ zmianê funkcji celu w kolenych iteracjach
	double m_last_objective_function;
	ObjectiveResult m_objective_result;
	int m_found_solutions;
	int m_same_counter;

private:
	const double MIN_PHEROMONE = 0.001;
	const double MAX_PHEROMONE = 0.999;

	const int MAX_SAME_COUNTER = 5;
};

