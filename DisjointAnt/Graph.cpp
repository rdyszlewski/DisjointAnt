#include "Graph.hpp"

Graph::Graph()
{
}

Graph::~Graph()
{
}

void Graph::Init(int* data, int verticesNumber, int* sources, int* targets) 
{
	m_vertices_number = verticesNumber;

	m_data = new std::map<int, Edge*>[verticesNumber];
	for (int i = 0; i < verticesNumber; i++) {
		m_data[i] = std::map<int, Edge*>();
	}

	int m_pairs_number = sizeof(sources) / sizeof(int);
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

	for (int i = 0; i < verticesNumber; i++) {
		for (int j = 0; j < verticesNumber; j++) {
			if (m_data[i].count(j)) {
				std::cout << m_data[i].at(j)->weight;
				std::cout << "\n";
			}
		}
	}
}

int Graph::GetSource(int sourceNumber) {
	if (sourceNumber >= 0 && sourceNumber < m_pairs_number) {
		return m_sources[sourceNumber];
	}
}

int Graph::GetTarget(int targetNumber) {
	if (targetNumber >= 0 && targetNumber < m_pairs_number) {
		return m_targets[targetNumber];
	}
}

int Graph::GetWeight(int firstVertex, int secondVertex) {
	if (m_data[firstVertex].count(secondVertex)) {
		return m_data[firstVertex].at(secondVertex)->weight;
	}
	return 0;
}

int Graph::GetPheromone(int firstVertex, int secondVertex, int colony) {
	if (m_data[firstVertex].count(secondVertex)) {
		return m_data[firstVertex].at(secondVertex)->pheromon[colony];
	}
	return 0;
}

int Graph::GetPairsNumber() {
	return m_pairs_number;
}

int Graph::GetVerticesNumber()
{
	return m_vertices_number;
}

std::vector<unsigned int> Graph::GetPossibleVertex(int vertex)
{
	std::vector<unsigned int> result;
	std::map<int, Edge*>::iterator it;
	for (it = m_data[vertex].begin(); it != m_data[vertex].end(); it++)
	{
		// TODO przemy�le� to 
		// TODO mo�na te� wstawi� tutaj dodatkowe warunki
		result.push_back(it->first);
	}
	return result;
}

void Graph::SetPheromone(int firstVertex, int secondVertex, int colony, double value) {
	m_data[firstVertex].at(secondVertex)->pheromon[colony] = value;
}

void Graph::AddPheromone(int firstVertex, int secondVertex, int colony, double value)
{
	m_data[firstVertex].at(secondVertex)->pheromon[colony] += value;
}

void Graph::ResetPheromones(double value)
{
	std::map<int, Edge*>::iterator it;
	for (int vertex = 0; vertex < m_vertices_number; vertex++)
	{
		for (it = m_data[vertex].begin(); it != m_data[vertex].end(); i++)
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
	//delete m_data; //TODO zobaczy� dlaczego nie moge tego usun��
	delete m_sources;
	delete m_targets;
}