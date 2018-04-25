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
	Init(graph, stepNumber, antInColony, alpha, p, beta);
	InitPheromones(m_graph);
	m_ants = CreateAnts(m_ant_number, m_number_colonies, m_graph);
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
	std::cout << CalculateObjectiveFunction();
}

void AntColony::SaveResult(int iteration, std::vector<int>& iterationResults)
{
	iterationResults.clear();
	// wynik dla ka¿dego mrowiska
	for (int colony = 0; colony < m_number_colonies; colony++)
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

	m_number_colonies = m_graph->GetPairsNumber();
	for (int i = 0;i< m_number_colonies; i++)
	{
		m_best_path.push_back(std::vector<unsigned int>());
		m_best_ant.push_back(nullptr);
		m_best_distance.push_back(std::numeric_limits<int>::max());
	}
	
	int numberVertices = m_graph->GetVerticesNumber();
	m_best_paths_matrix = new short*[numberVertices];
	for (int i = 0; i < numberVertices; i++)
	{
		m_best_paths_matrix[i] = new short[numberVertices];
		for (int j = 0; j < numberVertices; j++)
		{
			m_best_paths_matrix[i][j] = -1;
		}
	}

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
	ant->LookFor(m_graph, m_best_paths_matrix, m_alpha, m_beta);
}

void AntColony::ChooseBestPaths()
{
	ZeroBestPaths();
	for (Ant* ant : m_ants)
	{
		int colony = ant->GetColony();
		int distance = ant->GetDistance(m_graph);
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
		int size = path.size();
		for (int vertex = 0; vertex < size-1; vertex++) {
			int index1 = path[vertex];
			int index2 = path[vertex + 1];
			m_best_paths_matrix[index1][index2] = -1;
		}
	}
}

void AntColony::UpdateBestPaths()
{
	int vertices = m_graph->GetVerticesNumber();
	for (int colony = 0; colony< m_number_colonies; colony++)
	{
		int size = m_best_path[colony].size();
		for (int i = 0; i < size-1; i++)
		{
			int index1 = m_best_path[colony][i];
			int index2 = m_best_path[colony][i + 1];
			m_best_paths_matrix[index1][index2] = colony;
		}
	}
	for (int i = 0; i < vertices; i++)
	{
		for (int j = 0; j < vertices; j++)
		{
			std::cout << m_best_paths_matrix[i][j] << " ";
		}
		std::cout << std::endl;
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

void AntColony::FixPheromoneValue()
{
	const int numberColony = m_number_colonies;
	const int minPheromone = MIN_PHEROMONE;
	const int maxPheromone = MAX_PHEROMONE;
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