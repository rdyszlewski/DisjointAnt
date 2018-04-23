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
	m_ants = CreateAnts(m_ant_number, m_colony_number, m_graph);
	// p�tla wykonywana przez okre�lon� liczb� krok�w
	//TODO doda� jeszcze jeden warunek: je�eli nic si� nie zmieni�o przez okre�lon� liczb� krok�w ko�czymy
	for (int step = 0; step < m_step_number; step++)
	{
		StartAnts(); //startujemy mr�wki
		ChooseBestPath();
		ResetAnts();
	}

}

void AntColony::Init(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta)
{
	m_graph = graph;
	m_step_number = stepNumber;
	m_ant_number = antInColony;
	m_alpha = alpha;
	m_p = p;
	m_beta = beta;

	m_colony_number = m_graph->GetPairsNumber();
	for (int i = 0; m_colony_number; i++)
	{
		m_best_path[i] = std::vector<unsigned int>();
		m_best_ant[i] = nullptr;
		m_best_distance[i] = std::numeric_limits<int>::max();
	}
}

void AntColony::InitPheromones(Graph* graph)
{
	//TODO wszystko mo�na przenie�� do tej klasy
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
	//TODO zrobi� utworzenie nowego w�tku
	ant->LookFor(m_graph);
}

void AntColony::ChooseBestPath()
{
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