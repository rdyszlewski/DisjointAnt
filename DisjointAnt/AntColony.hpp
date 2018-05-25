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
		m_max_same_counter = counter;
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

	inline void AddBlockedColony(int colony)
	{
		m_blocked_colonies.push_back(colony);
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

	void HandlePheromone();
	void UpdatePheromone(double factor);
	/// odparowanie feromonu, korzystaj¹ce ze sta³ej funkcji 
	void EvaporatePheromone();
	void EvaporatePheromone(double factor);
	void FixPheromoneValue();

	void SaveResult(int iteration, int objectiveFunction);
	void ServeSameObjectiveFunction(int objectiveFunction);
	void SummarizeIteration(int iteration);

	void InitColoniesOrders(int numberColonies);
	void RandomColoniesOrders();
	int FindColonyOrderIndex(int colonyNumber);
	void InitLastPathsMatrix();
	void InitUsedEdgesMatrix();
	void InitBestResults(int numberColonies);
	void ClearUsedEdgesMatrix();

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
	double m_evaporate_factor;

	/// kolejnoœæ w jakiej mrówki bêd¹ startowaæ 
	std::vector<int> m_orders;

	/// mrowiska, które bêd¹ startowa³y jako pierwsze ( mrowiska, które w poprzedniej iteracji mialy zablokowan¹ œcie¿kê)
	std::vector<int> m_blocked_colonies;

	/// macierz œcie¿ek u¿ytych w danych iteracji. Macierz zawiera numery koloni, która zajumje dan¹ krawêdŸ
	/// to, które œcie¿ki bêd¹ uznawane za u¿yte zale¿y od metody aktualizacji feromonu(czy wszystkie mrówki aktualizuj¹, czy tylko najlepzse)
	short** m_used_edges_matrix;
	/// nie pamiêtam jak to dzia³a³o
	short** m_last_paths_matrix;
	/// najlepsze rozwi¹zania dla ka¿dego z mrowisk
	std::vector<std::vector<unsigned int>> m_best_paths;
	/// mrówki, które osi¹gnê³y najkrótsze œcie¿ki dla danego mrowiska
	std::vector<Ant*> m_best_ants;
	/// najkrótsze odleg³oœci
	std::vector<int> m_best_distances;

	std::vector<std::vector<unsigned int>> m_best_paths_results;

	WriteListener* m_write_listener;

	// zmienne pomagaj¹ce wykryæ zmianê funkcji celu w kolenych iteracjach
	/// wartoœæ ostatniej funkcji celui
	double m_last_objective_function;
	// TODO to ju¿ chyba jest niepotrzebne
	ObjectiveResult m_objective_result;
	/// liczba znalezionych rozwi¹zañ
	int m_found_solutions;
	
	/// licznik iteracji, w których nie zmieni³a siê funkcja celu
	int m_same_counter;

private:
	const double MIN_PHEROMONE = 0.001;
	const double MAX_PHEROMONE = 0.999;

	const int MAX_SAME_COUNTER = 20;

	const std::function<void(Graph::Edge*)> f_evaporate_function = [&](Graph::Edge* x)
	{
		for (int colony = 0; colony < m_number_colonies; colony++)
		{
			//x->pheromon[colony] -= m_evaporate_factor;
			x->pheromon[colony] -= x->pheromon[colony] * (1-m_p);
		}
	};
};

