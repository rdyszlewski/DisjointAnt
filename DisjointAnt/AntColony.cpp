#include "AntColony.hpp"



AntColony::AntColony(int steps):m_steps(steps), m_current_step(0)
{
}


AntColony::~AntColony()
{
}

void AntColony::Start(Graph* graph, int stepNumber, int antInColony, float alpha, float p, float beta) {
	m_graph = graph;
	m_step_number = stepNumber;
	m_ant_number = antInColony;
	m_alpha = alpha;
	m_p = p;
	m_beta = beta;

	m_colony_number = m_graph->GetPairsNumber();


}

void AntColony::InitPheromones(Graph* graph)
{
	//TODO wszystko mo¿na przenieœæ do tej klasy
	graph->ResetPheromones(MIN_PHEROMONE);
}

void AntColony::CreateAnts(int numberAnts, int numberColony)
{
	int totalAnts = 0;
	for (int colony = 0; colony < numberColony; colony++)
	{

	}
}
