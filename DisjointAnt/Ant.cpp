#include "Ant.h"

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

const uint Ant::GetDistance() const
{
	uint distance = 0;
	for (uint weight : m_path) {
		distance += weight;
	}
	return distance;
}

void Ant::UpdatePheromone(Graph* graph, double factor) {
	//TODO 
	// uaktualnienie feromonu na �cie�ce 
	int pathSize = m_path.size();
	for (int i = 0; i < pathSize-1; i++)
	{
		uint vertex1 = m_path[i];
		uint vertex2 = m_path[i + 1];
		//TODO przerobi� metod� na AddPheromone
		//TODO ogarn��, jaka warto�� feromonu powinna byc dodawana do �cie�ki
		uint value = CalculatePheromoneValue(vertex1, vertex2, factor, graph);
		graph->AddPheromone(vertex1, vertex2, m_colony, value);
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
void Ant::LookFor(Graph* graph, unsigned short** bestPaths)
{
	// TODO mo�e by� konieczne ustawienie warunku, �e mr�wka nie mo�e dotrze� do ko�ca
	while (m_current_vertex != m_end_vertex)
	{
		Step(graph, bestPaths);
	}
}

void Ant::Step(Graph* graph, unsigned short** bestPaths)
{
	bool findNext = false;
	while (!findNext)
	{
		std::vector<uint> possibleVertex = graph->GetPossibleVertex(m_current_vertex);
		possibleVertex = ExcludePossibleVertex(possibleVertex, bestPaths);
		if (!possibleVertex.empty())
		{
			uint vertex = RandomVertex(possibleVertex);
			Visit(vertex);
			findNext = true;
		}
		else
		{
			GoBack();
		}
	}	
}

std::vector<uint>& Ant::ExcludePossibleVertex(std::vector<uint>& possibleVertex, unsigned short** bestPaths)
{
	//TODO ustawi� poprane ograniczenia
	//TODO sprawdzi�, czy to dzia�a
	
	// usuwamy wierzcho�ki ju� odwiedzone 
	// oraz wierzho�ki zaj�te prez mr�wki z innego mrowiska
	possibleVertex.erase(std::remove_if(possibleVertex.begin(), possibleVertex.end(),
		[&](auto& x) {return m_visited_vertices[x]
		|| (bestPaths[m_current_vertex][x] != m_colony
			&& bestPaths[m_current_vertex][x] != 0); }),
		possibleVertex.end());


	return possibleVertex;
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

uint Ant::RandomVertex(std::vector<uint> possibleVertex) 
{
	//TODO zrobi� poprawne losowanie wierzcho�ka
	int possibleVertexSize = possibleVertex.size();
	int index = rand() % possibleVertexSize;
	return possibleVertex[index];
}

void Ant::GoBack()
{
	uint vertex = m_path.back();
	m_path.pop_back();
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