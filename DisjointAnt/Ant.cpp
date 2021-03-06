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

/// dodaje warto�� feromonu na kraw�dziach nale��cych do �cie�ki, przez kt�r� przesz�a 
/// mr�wka
//TODO okre�li� jaki� factor
void Ant::UpdatePheromone(Graph* graph, double factor) 
{
	int distance = GetDistance(graph);
	double pheromoneValue = factor / distance; // TODO sprawdzi�, czy type b�d� si� zgadza�
	//double pheromoneValue = factor * distance;
	int pathSize = m_path.size();
	for (int i = 0; i < pathSize-1; i++)
	{
		uint vertex1 = m_path[i];
		uint vertex2 = m_path[i + 1];
		graph->AddPheromone(vertex1, vertex2, m_colony, pheromoneValue);
	}
}

/// Rozpoczyna szukanie �cie�ki przez mr�wk� wed�ug algorytmu
/// dop�ki nie znaleziono odpowiedniego wyj�cia
///		sprawd� dok�d mo�na si� uda� z obecnego wierzcho�ka
///		je�eli znaleziono pasuj�cy wierzcho�ek
///			udaj si� do wybranego wierzcho�ka
///		je�eli nie znaleziono pasuj�cego wierzcho�ka
///			wr�� do poprzedniego wierzcho�ka
///			powt�rz szukanie 
bool Ant::LookFor(Graph* graph, short** usedPaths, double alpha, double beta)
{
	m_path.push_back(m_home_vertex);
	m_current_vertex = m_home_vertex;
	// TODO mo�e by� konieczne ustawienie warunku, �e mr�wka nie mo�e dotrze� do ko�ca
	bool leaveHome = false;
	while (m_current_vertex != m_end_vertex )
	{
		Step(graph, usedPaths, alpha, beta);
		leaveHome = true;
		if (leaveHome && m_current_vertex == m_home_vertex)
		{
			return false;
		}
	}
	return true;
}

void Ant::Step(Graph* graph, short** usedPaths, double alpha, double beta)
{
	bool findNext = false;
	while (!findNext)
	{
		std::vector<uint> possibleVertex = graph->GetPossibleVertex(m_current_vertex);
		possibleVertex = ExcludePossibleVertex(possibleVertex, usedPaths);
		if (!possibleVertex.empty())
		{
			//TODO mo�na tutaj jako� uwzgl�dnia� to, �eby mr�wki nie pr�bowa�y wchodzi�
			// do nie swoich wyj��
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

std::vector<uint> Ant::ExcludePossibleVertex(std::vector<uint>& possibleVertex, short** usedPaths)
{
	// je�li odwiedzi�a ju� t� kraw�d�
	// je�eli �cie�ka jest zaj�ta przez inne mrowisko

	/*std::vector<uint> result;
	for (uint vertex : possibleVertex)
	{
		//TODO ubra� to w jakie� �adne metody
		if (!m_visited_vertices[vertex] && (
			bestPaths[m_current_vertex][vertex] == m_colony
			|| bestPaths[m_current_vertex][vertex] == -1
			)) 
		{
			result.push_back(vertex);
		}
	}*/

	std::vector<uint> result;
	for (uint vertex : possibleVertex)
	{
		//TODO ubra� to w jakie� �adne metody
		if (!m_visited_vertices[vertex] && (
			usedPaths[m_current_vertex][vertex] == m_colony
			|| usedPaths[m_current_vertex][vertex] == -1
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

//TODO przetestowa� to wszystko
uint Ant::RandomVertex(Graph* graph, std::vector<uint> possibleVertex, double alpha, double beta) 
{
	//TODO zrobi� poprawne losowanie wierzcho�ka
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
	double value = (double)randInt / 100.0; //TODO sprawdzi�, czy b�dzie zwr�cony odpowiedni typ

	for (int i = size-1; i >=0; i--)
	{
		if (probabilites[i] < value)
		{
			return possibleVertex[i+1];
		}
	}
	return possibleVertex[0]; //zwracamy ostatni� warto��
}

//TODO zmieni� nazw�, poniewa� to nie liczy prawdopodobie�stwa
double Ant::CalculateProbability(double alpha, double beta, double pheromone, int distance)
{
	assert(distance > 0);
	if (pheromone > 0.001)
	{
		std::cout << "";
	}
	double ETA = (double)pow(1.0/(double)distance, beta);
	double TAU = (double)pow(pheromone, alpha);
	return TAU * ETA;
	/*return
		((double)pow(pheromone, alpha) * (double)(pow(1 / (double)distance, beta)));*/
}

void Ant::GoBack()
{
	m_path.pop_back();
	uint vertex = m_path.back();
	m_path.pop_back(); //�ci�gamy te� wierzcho�ek, kt�ry zaraz zostanie odwiedzony
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
	
	//TODO napisa� wzorek
	//TODO tutaj mo�e by� rozr�nienie, czy aktualizujemy ka�d� wage odzielnie, czy wszystko razem
	return factor;
}