#include "AntColony.hpp"
#include "WriteListener.hpp"

//TODO przypatrzeæ siê, czy te stepsy w tym miejsu maj¹ sens
AntColony::AntColony(int steps):m_steps(steps), m_current_step(0)
{
}


AntColony::~AntColony()
{
}

void AntColony::Start(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta)
{
	m_evaporate_factor = 0.1; //TODO przenieœæ to do jakiegoœ parametru
	Init(graph, stepNumber, antInColony, alpha, p, beta);
	InitPheromones(m_graph);
	m_ants = CreateAnts(m_ant_number, m_number_colonies, m_graph);
	// pêtla wykonywana przez okreœlon¹ liczbê kroków
	//TODO dodaæ jeszcze jeden warunek: je¿eli nic siê nie zmieni³o przez okreœlon¹ liczbê kroków koñczymy
	std::vector<int> iterationResults;
	int iteration = 0;
	// algorytm wykonywany jest przez pewn¹ iloœæ iteracji, lub dopóki wartoœæ funkcji celu nie zmieni
	// siê przez okreœlon¹ liczbê iteracji
	while(iteration != m_step_number && m_same_counter != m_max_same_counter)
	{
		m_found_solutions = 0;
		// mrówki z kolejnego mrowiska s¹ puszczane dopiero po zakoñczeniu dzia³añ
		// przez mrówki z poprzedniego mrowiska
		for (int order = 0; order < m_number_colonies; order++)
		{
			int colony = m_orders[order];
			StartAnts(colony);
			ChooseBestPathInIteration(colony);
		}

		HandlePheromone();
		SummarizeIteration(iteration);
		ResetAnts();
		RandomColoniesOrders(); //TODO dodaæ tutaj blokowane 
		ClearUsedEdgesMatrix();
		iteration++;
	}
}

void AntColony::HandlePheromone()
{
	UpdatePheromone(0.5);
	EvaporatePheromone();
	FixPheromoneValue();
}

void AntColony::SummarizeIteration(int iteration)
{
	int objectiveFunctionResult = CalculateObjectiveFunction();
	ServeSameObjectiveFunction(objectiveFunctionResult);
	ObjectiveResult newResult;
	newResult.distance = objectiveFunctionResult;
	newResult.numberPaths = m_found_solutions;
	if (CompareObjectiveFunction(newResult, m_objective_result))
	{
		m_objective_result = newResult;
		CopyBestPathsResults(m_best_paths_results, m_best_paths);
	}

	if (m_write_listener != nullptr)
	{
		SaveResult(iteration, objectiveFunctionResult);
	}
}

void AntColony::CopyBestPathsResults(std::vector<std::vector<unsigned int>>& result, std::vector<std::vector<unsigned int>>& source)
{
	//TODO to prawdopodobnie bêdzie dzia³aæ Ÿle, poniewa¿ przepisujemy referencje
	result = source;
	
	/*for (int i = 0; i < m_number_colonies; i++)
	{
		result[i].clear();
		for (int j = 0; j < source[i].size(); j++)
		{
			result[i].push_back(source[i][j]);
		}
	}*/
}

void AntColony::ServeSameObjectiveFunction(int objectiveFunction)
{
	if (m_last_objective_function == objectiveFunction)
	{
		m_same_counter++;
	}
	else {
		m_same_counter = 0;
		m_last_objective_function = objectiveFunction;
	}
}

void AntColony::SaveResult(int iteration, int objectiveFunction)
{
	std::vector<int> iterationResults;
	// wynik dla ka¿dego mrowiska
	for (int colony = 0; colony < m_number_colonies; colony++)
	{
		iterationResults.push_back(m_best_distances[colony]);
	}
	//wynik funkcji celu
	iterationResults.push_back(objectiveFunction);
	m_write_listener->WriteResult(iteration, iterationResults);
}

void AntColony::Init(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta)
{
	m_graph = graph;
	m_step_number = stepNumber;
	m_ant_number = antInColony;
	m_alpha = alpha;
	m_p = p;
	m_beta = beta;
	m_last_objective_function = 0;
	m_same_counter = 0;
	m_max_same_counter = MAX_SAME_COUNTER;

	m_number_colonies = m_graph->GetPairsNumber();
	InitBestResults(m_number_colonies);
	InitLastPathsMatrix();
	InitUsedEdgesMatrix();
	InitColoniesOrders(m_number_colonies);
	RandomColoniesOrders();

	for (int i = 0; i < graph->GetVerticesNumber(); i++) {

	}
}

void AntColony::InitColoniesOrders(int numberColonies)
{
	for (int i = 0; i < numberColonies; i++)
	{
		m_orders.push_back(i);
	}
}

void AntColony::InitBestResults(int numberColonies)
{
	for (int i = 0; i < numberColonies; i++)
	{
		m_best_paths.push_back(std::vector<unsigned int>());
		m_best_ants.push_back(nullptr);
		m_best_distances.push_back(std::numeric_limits<int>::max());
	}
}

void AntColony::InitUsedEdgesMatrix()
{
	int numberVertices = m_graph->GetVerticesNumber();
	m_used_edges_matrix = new short*[numberVertices];
	for (int i = 0; i < numberVertices; i++)
	{
		m_used_edges_matrix[i] = new short[numberVertices];
		for (int j = 0; j < numberVertices; j++)
		{
			m_used_edges_matrix[i][j] = -1;
		}
	}
}

void AntColony::ClearUsedEdgesMatrix()
{
	const int verticesNumber = m_graph->GetVerticesNumber();
	std::for_each(m_used_edges_matrix, m_used_edges_matrix + verticesNumber, [&](short* x)
	{
		for (int i = 0; i < verticesNumber; i++)
		{
			x[i] = -1;
		}
	});
}

void AntColony::InitLastPathsMatrix()
{
	int numberVertices = m_graph->GetVerticesNumber();
	m_last_paths_matrix = new short*[numberVertices];
	for (int i = 0; i < numberVertices; i++)
	{
		m_last_paths_matrix[i] = new short[numberVertices];
		for (int j = 0; j < numberVertices; j++)
		{
			m_last_paths_matrix[i][j] = -1;
		}
	}
}

void AntColony::RandomColoniesOrders()
{
	// TODO przetestowaæ jak to dzia³a
	int blockedColonies = m_blocked_colonies.size();
	if (blockedColonies > 0)
	{
		std::cout << "Wyst¹pi³o blokowanie";
	}
	// wstawianie blokowanych mrowisk na pierwsze pozycji
	for (int i = 0; i < blockedColonies; i++)
	{
		int orderIndex = FindColonyOrderIndex(m_blocked_colonies[i]);
		//std::swap(m_orders[i], m_orders[orderIndex]);
		std::iter_swap(m_orders.begin() + i, m_orders.begin() + orderIndex);
	}
	//std::random_shuffle(m_orders.begin(), m_orders.end());
	// losowanie odbywa siê zostawiaj¹c blokowane kolonie
	std::random_shuffle(m_orders.begin() + blockedColonies, m_orders.end());
}

int AntColony::FindColonyOrderIndex(int colonyNumber)
{
	int ordersSize = m_orders.size();
	for (int i = 0; i < ordersSize; i++)
	{
		if (m_orders[i] == colonyNumber) {
			return i;
		}
	}
	return -1;
}

void AntColony::InitPheromones(Graph* graph)
{
	//TODO wszystko mo¿na przenieœæ do tej klasy
	const int numberColonies = m_number_colonies;
	const double minPheromone = MIN_PHEROMONE;

	std::function<void(Graph::Edge*)> func = [&](Graph::Edge* x)
	{
		for (int colony = 0; colony < numberColonies; colony++)
		{
			x->pheromon[colony] = minPheromone;
		}
	};

	graph->ForEach(func);
}

std::vector<std::vector<Ant*>> AntColony::CreateAnts(int numberAnts, int numberColonies, Graph* graph)
{
	std::vector<std::vector<Ant*>> result;
	int totalAnts = 0;
	Ant* newAnt;
	int numberVertices = graph->GetVerticesNumber();
	for (int colony = 0; colony < numberColonies; colony++)
	{
		std::vector<Ant*> ants;
		int homeVertex = graph->GetSource(colony);
		int endVertex = graph->GetTarget(colony);
		for (int ant = 0; ant < numberAnts; ant++)
		{
			newAnt = new Ant(totalAnts, colony, ant, homeVertex, endVertex, numberVertices);
			ants.push_back(newAnt);
		}
		result.push_back(std::move(ants));
	}
	return result;
}

void AntColony::StartAnts(int colony)
{
	std::for_each(m_ants[colony].begin(), m_ants[colony].end(), [&](auto x)
	{
		StartAnt(x);
	});
}

void AntColony::StartAnt(Ant* ant)
{
	//ant->LookFor(m_graph, m_last_paths_matrix, m_alpha, m_beta);
	ant->LookFor(m_graph, m_used_edges_matrix, m_alpha, m_beta);
}

void AntColony::ChooseBestPathInIteration(int colony)
{
	//TODO sprawdziæ tutaj 
	ZeroBestPath(colony);
	int bestDistance = std::numeric_limits<int>::max();
	for (Ant* ant : m_ants[colony])
	{
		int distance = ant->GetDistance(m_graph);
		if (distance > 0)
		{
			m_found_solutions++;
		} 
		else
		{
			m_blocked_colonies.push_back(colony);
		}
		if (distance < bestDistance)
		{
			bestDistance = distance;
			m_best_distances[colony] = distance;
			m_best_ants[colony] = ant;
			m_best_paths[colony] = ant->GetPath();
		}
	}
	UpdateBestPath(colony);
}

void AntColony::ZeroBestPath(int colony)
{
	int vertices = m_graph->GetVerticesNumber();
	int size = m_best_paths[colony].size();
	for (int vertex = 0; vertex < size - 1; vertex++)
	{
		int index1 = m_best_paths[colony][vertex];
		int index2 = m_best_paths[colony][vertex + 1];
		m_last_paths_matrix[index1][index2] = -1;
	}
}

void AntColony::UpdateBestPath(int colony)
{
	int size = m_best_paths[colony].size();
	for (int i = 0; i < size - 1; i++)
	{
		int index1 = m_best_paths[colony][i];
		int index2 = m_best_paths[colony][i + 1];
		m_last_paths_matrix[index1][index2] = colony;
	}
}

void AntColony::ResetAnts()
{
	for (int colony = 0; colony < m_number_colonies; colony++)
	{
		for (Ant* ant : m_ants[colony])
		{
			ant->Reset();
		}
	}
}

void AntColony::ReleaseAnts()
{
	for (int colony = 0; colony < m_number_colonies; colony++)
	{
		for (Ant* ant : m_ants[colony])
		{
			delete ant;
		}
		m_ants[colony].clear();
	}
	m_ants.clear();
}

int AntColony::CalculateObjectiveFunction()
{
	int result = 0;
	int pairsNumber = m_graph->GetPairsNumber();
	for (int i = 0; i < pairsNumber; i++)
	{
		result += m_best_distances[i];
	}
	return result;
}

void AntColony::EvaporatePheromone(double factor)
{
	const int numberColony = m_number_colonies;
	std::function<void(Graph::Edge*)> func = [&](Graph::Edge* x)
	{
		for (int colony = 0; colony < numberColony; colony++)
		{
			x->pheromon[colony] -= factor;
		}
	};
	
	m_graph->ForEach(func);
}

void AntColony::EvaporatePheromone() {
	m_graph->ForEach(f_evaporate_function);
}

void AntColony::UpdatePheromone(double factor)
{
	/*for (int i = 0; i < m_number_colonies; i++)
	{
		Ant* ant = m_best_ants[i];
		ant->UpdatePheromone(m_graph, factor);
	}*/

	// aktualizacja feromonu dla ka¿dej mrówki
	// aktualizacja feromonu powinna byæ zale¿na od d³ugoœci œcie¿ki
	for (int i = 0; i < m_number_colonies; i++) {
		std::for_each(m_ants[i].begin(), m_ants[i].end(), [&](auto x) {
			x->UpdatePheromone(m_graph, factor);
		});
	}
}


void AntColony::FixPheromoneValue()
{
	const int numberColony = m_number_colonies;
	const double minPheromone = MIN_PHEROMONE;
	const double maxPheromone = MAX_PHEROMONE;
	std::function<void(Graph::Edge*)> func = [&](Graph::Edge* x)
	{
		for (int colony = 0; colony < numberColony; colony++)
		{
			if (x->pheromon[colony] < minPheromone)
			{
				x->pheromon[colony] = minPheromone;
			} 
			else if(x->pheromon[colony] > maxPheromone)
			{
				x->pheromon[colony] = maxPheromone;
			}
		}
	};
	m_graph->ForEach(func);
}

bool AntColony::CompareObjectiveFunction(ObjectiveResult result1, ObjectiveResult result2)
{
	if (result1.numberPaths > result2.numberPaths)
	{
		return true;
	}
	else if (result1.numberPaths == result2.numberPaths)
	{
		return result1.distance < result2.distance;
	}
	return false;
}