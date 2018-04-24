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
	// uaktualnienie feromonu na œcie¿ce 
	int pathSize = m_path.size();
	for (int i = 0; i < pathSize-1; i++)
	{
		uint vertex1 = m_path[i];
		uint vertex2 = m_path[i + 1];
		//TODO przerobiæ metodê na AddPheromone
		//TODO ogarn¹æ, jaka wartoœæ feromonu powinna byc dodawana do œcie¿ki
		uint value = CalculatePheromoneValue(vertex1, vertex2, factor, graph);
		graph->AddPheromone(vertex1, vertex2, m_colony, value);
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
void Ant::LookFor(Graph* graph, unsigned short** bestPaths)
{
	// TODO mo¿e byæ konieczne ustawienie warunku, ¿e mrówka nie mo¿e dotrzeæ do koñca
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
	//TODO ustawiæ poprane ograniczenia
	//TODO sprawdziæ, czy to dzia³a
	
	// usuwamy wierzcho³ki ju¿ odwiedzone 
	// oraz wierzho³ki zajête prez mrówki z innego mrowiska
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
	//TODO zrobiæ poprawne losowanie wierzcho³ka
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
	//TODO napisaæ wzorek
	//TODO tutaj mo¿e byæ rozró¿nienie, czy aktualizujemy ka¿d¹ wage odzielnie, czy wszystko razem
	return factor;
}