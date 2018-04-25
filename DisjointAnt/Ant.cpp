#include "Ant.h"
#include <math.h>
#include <cassert>
#include <random>
#include <algorithm>

Ant::Ant(uint id, uint colony, uint numberInColony, 
	uint homeVertex, uint endVertex, uint numberVertices):
	m_id(id), m_colony(colony), m_number_in_colony(numberInColony),
	m_home_vertex(homeVertex), m_end_vertex(endVertex)
{
	InitVisitedVertices(numberVertices);
}


Ant::~Ant()
{
	delete m_visited_vertices;
}

const uint Ant::GetDistance(Graph* graph) const
{
	uint distance = 0;
	int size = m_path.size();
	
	for (int i = 0; i < size - 1; i++)
	{
		distance += graph->GetWeight(m_path[i], m_path[i + 1]);
	}
	return distance;
}

/// dodaje wartoœæ feromonu na krawêdziach nale¿¹cych do œcie¿ki, przez któr¹ przesz³a 
/// mrówka
//TODO okreœliæ jakiœ factor
void Ant::UpdatePheromone(Graph* graph, double factor) {
	//TODO 
	// uaktualnienie feromonu na œcie¿ce 
	/*int pathSize = m_path.size();
	for (int i = 0; i < pathSize-1; i++)
	{
		uint vertex1 = m_path[i];
		uint vertex2 = m_path[i + 1];
		//TODO przerobiæ metodê na AddPheromone
		//TODO ogarn¹æ, jaka wartoœæ feromonu powinna byc dodawana do œcie¿ki
		uint value = CalculatePheromoneValue(vertex1, vertex2, factor, graph);
		graph->AddPheromone(vertex1, vertex2, m_colony, value);
	}*/

	int distance = GetDistance(graph);
	double pheromoneValue = factor / distance; // TODO sprawdziæ, czy type bêd¹ siê zgadzaæ
	int pathSize = m_path.size();
	for (int i = 0; i < pathSize-1; i++)
	{
		uint vertex1 = m_path[i];
		uint vertex2 = m_path[i + 1];
		graph->AddPheromone(vertex1, vertex2, m_colony, pheromoneValue);
	}
}


/// Rozpoczyna szukanie œcie¿ki przez mrówkê wed³ug algorytmu
/// dopóki nie znaleziono odpowiedniego wyjœcia
///		sprawdŸ dok¹d mo¿na siê udaæ z obecnego wierzcho³ka
///		je¿eli znaleziono pasuj¹cy wierzcho³ek
///			udaj siê do wybranego wierzcho³ka
///		je¿eli nie znaleziono pasuj¹cego wierzcho³ka
///			wróæ do poprzedniego wierzcho³ka
///			powtórz szukanie 
void Ant::LookFor(Graph* graph, short** bestPaths, double alpha, double beta)
{
	m_path.push_back(m_home_vertex);
	// TODO mo¿e byæ konieczne ustawienie warunku, ¿e mrówka nie mo¿e dotrzeæ do koñca
	while (m_current_vertex != m_end_vertex)
	{
		Step(graph, bestPaths, alpha, beta);
	}
}

void Ant::Step(Graph* graph, short** bestPaths, double alpha, double beta)
{
	bool findNext = false;
	while (!findNext)
	{
		std::vector<uint> possibleVertex = graph->GetPossibleVertex(m_current_vertex);
		possibleVertex = ExcludePossibleVertex(possibleVertex, bestPaths);
		if (!possibleVertex.empty())
		{
			//TODO mo¿na tutaj jakoœ uwzglêdniaæ to, ¿eby mrówki nie próbowa³y wchodziæ
			// do nie swoich wyjœæ
			uint vertex = RandomVertex(graph, possibleVertex,alpha, beta);
			Visit(vertex);
			findNext = true;
		}
		else
		{
			GoBack();
		}
	}	
}

std::vector<uint> Ant::ExcludePossibleVertex(std::vector<uint>& possibleVertex, short** bestPaths)
{
	//TODO ustawiæ poprane ograniczenia
	//TODO sprawdziæ, czy to dzia³a
	
	// usuwamy wierzcho³ki ju¿ odwiedzone 
	// oraz wierzho³ki zajête prez mrówki z innego mrowiska
	/*possibleVertex.erase(std::remove_if(possibleVertex.begin(), possibleVertex.end(),
		[&](auto& x) {return m_visited_vertices[x]
		|| (bestPaths[m_current_vertex][x] != m_colony
			&& bestPaths[m_current_vertex][x] != 0); }),
		possibleVertex.end());*/

	std::vector<uint> result;
	for (uint vertex : possibleVertex)
	{
		//TODO ubraæ to w jakieœ ³adne metody
		if (!m_visited_vertices[vertex] && (
			bestPaths[m_current_vertex][vertex] == m_colony
			|| bestPaths[m_current_vertex][vertex] == -1
			)) 
		{
			result.push_back(vertex);
		}
	}

	return std::move(result);
}

void Ant::Reset()
{
	m_path.clear();
	if (m_visited_vertices) {
		for (int i = 0; i < m_number_vertices; i++) {
			m_visited_vertices[i] = false;
		}
	}
	m_current_vertex = m_home_vertex;
}

void Ant::Visit(uint vertex) 
{
	m_path.push_back(vertex);
	m_visited_vertices[vertex] = true;
	m_current_vertex = vertex;
}

//TODO przetestowaæ to wszystko
uint Ant::RandomVertex(Graph* graph, std::vector<uint> possibleVertex, double alpha, double beta) 
{
	//TODO zrobiæ poprawne losowanie wierzcho³ka
	std::vector<double> probabilites; 
	double sum = 0;

	for (uint vertex : possibleVertex)
	{
		double pheromone = graph->GetPheromone(m_current_vertex, vertex, m_colony);
		int distance = graph->GetWeight(m_current_vertex, vertex);
		double probability = CalculateProbability(alpha, beta, pheromone, distance);
		probabilites.push_back(sum+ probability);
		sum += probability;
	}
	int size = probabilites.size();
	for (int i = 0; i < size; i++)
	{
		probabilites[i] /= sum;
	}
	
	int randInt = rand() % 100;
	double value = (double)randInt / 100.0; //TODO sprawdziæ, czy bêdzie zwrócony odpowiedni typ

	for (int i = 0; i < size; i++)
	{
		if (probabilites[i] < value)
		{
			return possibleVertex[i];
		}
	}
	return possibleVertex[size-1]; //zwracamy ostatni¹ wartoœæ
}

//TODO zmieniæ nazwê, poniewa¿ to nie liczy prawdopodobieñstwa
double Ant::CalculateProbability(double alpha, double beta, double pheromone, int distance)
{
	assert(distance > 0);
	return
		(pow(pheromone, alpha) * pow(1 / (float)distance, beta));
}

void Ant::GoBack()
{
	m_path.pop_back();
	uint vertex = m_path.back();
	m_path.pop_back(); //œci¹gamy te¿ wierzcho³ek, który zaraz zostanie odwiedzony
	Visit(vertex);
}

void Ant::InitVisitedVertices(uint numberVertices)
{
	m_number_vertices = numberVertices;
	m_visited_vertices = new bool[numberVertices];
	for (int i = 0; i < numberVertices; i++)
	{
		m_visited_vertices[i] = false;
	}
}

double Ant::CalculatePheromoneValue(uint vertex1, uint vertex2, double factor, Graph* graph)
{
	uint weight = graph->GetWeight(vertex1, vertex2);
	//TODO napisaæ wzorek
	//TODO tutaj mo¿e byæ rozró¿nienie, czy aktualizujemy ka¿d¹ wage odzielnie, czy wszystko razem
	return factor;
}