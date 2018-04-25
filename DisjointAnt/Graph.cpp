#include "Graph.hpp"
#include <algorithm>

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::Init(int* data, int verticesNumber, int* sources, int* targets, int k) 
{
	m_vertices_number = verticesNumber;

	m_data = new std::map<int, Edge*>[verticesNumber];
	for (int i = 0; i < verticesNumber; i++) {
		m_data[i] = std::map<int, Edge*>();
	}

	m_pairs_number = k;
	m_sources = new int[m_pairs_number];
	for (int i = 0; i < m_pairs_number; i++) {
		m_sources[i] = sources[i];
	}

	m_targets = new int[m_pairs_number];
	for (int i = 0; i < m_pairs_number; i++) {
		m_targets[i] = targets[i];
	}

	Edge* edge;
	for (int i = 0; i < verticesNumber; i++) {
		for (int j = 0; j < verticesNumber; j++) {
			int value = data[i*verticesNumber + j];
			if (value > 0) {
				edge = new Edge(i, j, value, verticesNumber);
				m_data[i].insert(std::make_pair(j, edge));
			}
		}
	}

	/*for (int i = 0; i < verticesNumber; i++) {
		for (int j = 0; j < verticesNumber; j++) {
			if (m_data[i].count(j)) {
				std::cout << m_data[i].at(j)->weight;
				std::cout << "\n";
			}
		}
	}*/
}

std::vector<unsigned int> Graph::GetPossibleVertex(int vertex)
{
	std::vector<unsigned int> result;
	std::map<int, Edge*>::iterator it;
	for (it = m_data[vertex].begin(); it != m_data[vertex].end(); it++)
	{
		// TODO przemyœleæ to 
		// TODO mo¿na te¿ wstawiæ tutaj dodatkowe warunki
		result.push_back(it->first);
	}
	return result;
}

void Graph::ResetPheromones(double value)
{
	std::map<int, Edge*>::iterator it;
	for (int vertex = 0; vertex < m_vertices_number; vertex++)
	{
		for (it = m_data[vertex].begin(); it != m_data[vertex].end(); it++)
		{
			for (int colony = 0; colony < m_pairs_number; colony++)
			{
				it->second->pheromon[colony] = value;
			}
		}
	}
}

void Graph::Release() 
{
	for (int i = 0; i < m_vertices_number; i++) {
		for (int j = 0; j < m_vertices_number; j++) {
			if (m_data[i].count(j)) {
				delete m_data[i].at(j)->pheromon;

			}
		}
		m_data[i].clear();
	}
	//delete m_data; //TODO zobaczyæ dlaczego nie moge tego usun¹æ
	delete m_sources;
	delete m_targets;
}

/// Wykonuje podan¹ w parametrze akcje dla ka¿dej krawêdzi w grafie
void Graph::ForEach(std::function<void(Graph::Edge*)> lambda)
{
	std::for_each(m_data, m_data + (int)m_vertices_number, [&](std::map<int, Edge*>& x) {
		std::for_each(x.begin(), x.end(), [&](auto& y) {
			lambda(y.second);
		});
	});
}