#include "AntColony.hpp"



AntColony::AntColony(int steps):m_steps(steps), m_current_step(0)
{
}


AntColony::~AntColony()
{
}

void AntColony::Start(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta) {
	
	Init(graph, stepNumber, antInColony, alpha, p, beta);
	InitPheromones(m_graph);
	m_ants = CreateAnts(m_ant_number, m_number_colony, m_graph);
	// pêtla wykonywana przez okreœlon¹ liczbê kroków
	//TODO dodaæ jeszcze jeden warunek: je¿eli nic siê nie zmieni³o przez okreœlon¹ liczbê kroków koñczymy
	std::vector<int> iterationResults;
	for (int step = 0; step < m_step_number; step++)
	{
		StartAnts(); //startujemy mrówki
		ChooseBestPaths();
		ResetAnts();

		if (m_write_listener != nullptr)
		{
			SaveResult(step, iterationResults);
		}
	}
}

void AntColony::SaveResult(int iteration, std::vector<int>& iterationResults)
{
	iterationResults.clear();
	// wynik dla ka¿dego mrowiska
	for (int colony = 0; colony < m_number_colony; colony++)
	{
		iterationResults.push_back(m_best_distance[colony]);
	}
	//wynik funkcji celu
	int objectiveFunctionResult = CalculateObjectiveFunction();
	iterationResults.push_back(objectiveFunctionResult);
	m_write_listener->writeResult(iteration, iterationResults);
}

void AntColony::Init(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta)
{
	m_graph = graph;
	m_step_number = stepNumber;
	m_ant_number = antInColony;
	m_alpha = alpha;
	m_p = p;
	m_beta = beta;

	m_number_colony = m_graph->GetPairsNumber();
	for (int i = 0; m_number_colony; i++)
	{
		m_best_path[i] = std::vector<unsigned int>();
		m_best_ant[i] = nullptr;
		m_best_distance[i] = std::numeric_limits<int>::max();
	}

	int numberVertices = m_graph->GetVerticesNumber();
	m_best_paths_matrix = new unsigned short*[numberVertices];
	for (int i = 0; i < numberVertices; i++)
	{
		m_best_paths_matrix[i] = new unsigned short[numberVertices];
	}

}

void AntColony::InitPheromones(Graph* graph)
{
	//TODO wszystko mo¿na przenieœæ do tej klasy
	graph->ResetPheromones(MIN_PHEROMONE);
}

std::vector<Ant*> AntColony::CreateAnts(int numberAnts, int numberColony, Graph* graph)
{
	std::vector<Ant*> result;
	int totalAnts = 0;
	Ant* newAnt;
	int numberVertices = graph->GetVerticesNumber();
	for (int colony = 0; colony < numberColony; colony++)
	{
		int homeVertex = graph->GetSource(colony);
		int endVertex = graph->GetTarget(colony);
		for (int ant = 0; ant < numberAnts; ant++)
		{
			newAnt = new Ant(totalAnts, colony, ant, homeVertex, endVertex, numberVertices);
			result.push_back(newAnt);
		}
	}
	return result;
}

void AntColony::StartAnts()
{
	for (Ant* ant : m_ants)
	{
		StartAnt(ant);
	}
}

void AntColony::StartAnt(Ant* ant)
{
	//TODO zrobiæ utworzenie nowego w¹tku
	ant->LookFor(m_graph, m_best_paths_matrix);
}

void AntColony::ChooseBestPaths()
{
	ZeroBestPaths();
	for (Ant* ant : m_ants)
	{
		int colony = ant->GetColony();
		int distance = ant->GetDistance();
		if (distance < m_best_distance[colony]) {
			m_best_distance[colony] = distance;
			m_best_ant[colony] = ant;
			m_best_path[colony]= ant->GetPath();
		}
	}
	UpdateBestPaths();
}

void AntColony::ZeroBestPaths()
{
	int vertices = m_graph->GetVerticesNumber();
	for (std::vector<uint> path: m_best_path)
	{
		for (int vertex = 0; vertex < vertices-1; vertex++) {
			m_best_paths_matrix[vertex][vertex + 1] = 0;
		}
	}
}

void AntColony::UpdateBestPaths()
{
	int vertices = m_graph->GetVerticesNumber();
	for (int colony = 0; colony< m_number_colony; colony++)
	{
		for (int vertex = 0; vertex < vertices - 1; vertex++) {
			m_best_paths_matrix[vertex][vertex + 1] = colony;
		}
	}
}

void AntColony::ResetAnts()
{
	for (Ant* ant : m_ants)
	{
		ant->Reset();
	}
}

void AntColony::ReleaseAnts()
{
	for (Ant* ant : m_ants)
	{
		delete ant;
	}
	m_ants.clear();
}

int AntColony::CalculateObjectiveFunction()
{
	int result = 0;
	int pairsNumber = m_graph->GetPairsNumber();
	for (int i = 0; i < pairsNumber; i++)
	{
		result += m_best_distance[i];
	}
	return result;
}

void AntColony::SetWriteListener(WriteListener* writeListener)
{
	m_write_listener = writeListener;
}