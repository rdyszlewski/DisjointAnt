#include "Graph.hpp"
#include "AntColony.hpp"
#include "Logger.hpp"
#include "ResultWriter.hpp"
#include "WriteListener.hpp"
#include "Timer.hpp"
#include <time.h>


class Listener : public WriteListener 
{
public:
	Listener(Logger& logger, ResultWriter& writer) :m_logger(logger), m_writer(writer) {}
	
	void WriteResult(int iteration, std::vector<int> values)
	{
		m_writer.WriteResult(iteration, values);
	};
	void WriteLog(std::string message)
	{
		m_logger.WriteLog(message);
	};

private:
	Logger& m_logger;
	ResultWriter& m_writer;
};

void print(Graph::Edge* x)
{
	std::cout << x->weight;
}

int main()
{
	const int STEP_NUMBER = 50;
	const int ANTS_IN_COLONY = 20;
	const float ALPHA = 1;
	const float P = 0.75;
	const float BETA =1;

	std::srand(time(NULL));
	Graph graph = Graph();
	//int data[] = { 0,0,3,0,0,0,0,1,0,0,0,0,0,4,1,0,0,0,0,0,0,0,0,0,0 };
	//int source[] = { 0,1 };
	//int target[] = { 3,4 };

	//int data[] = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 6, 12, 0, 0, 0, 0, 0, 4, 5, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//int source[] = { 0, 1 };
	//int target[] = { 5, 6 };

	//int data[] = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//int source[] = { 2, 1, 0 };
	//int target[] = { 9, 10, 8 };

	int data[] = { 0, 0, 0, 0, 3, 3, 3, 4, 7, 7, 0, 7, 11, 11, 11, 12, 0, 0, 0, 0, 2, 2, 0, 0, 6, 6, 7, 6, 10, 10, 10, 11, 0, 0, 0, 0, 1, 1, 1, 1, 5, 5, 5, 5, 9, 9, 9, 0, 0, 0, 0, 0, 1, 1, 0, 1, 4, 4, 4, 0, 8, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 3, 4, 4, 3, 7, 7, 7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 3, 6, 6, 7, 6, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 4, 4, 5, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int source[] = { 0,1 };
	int target[] = { 14, 15 };
	int VERTICES_SIZE = 16;
	int SOURCES_SIZE = 2;
	
	Logger logger;
	logger.OpenLogFile("D://log.txt");
	ResultWriter writer;
	writer.OpenResultFile("D://", "wynik.csv", ';');
	Listener* listener =new  Listener(logger, writer);
	WriteListener* writeListener = listener;
	graph.Init(data, VERTICES_SIZE, source, target, SOURCES_SIZE);
	AntColony antColony(50);
	antColony.SetWriteListener(writeListener);
	Timer timer;
	timer.Start("all");

	antColony.Start(&graph, STEP_NUMBER, ANTS_IN_COLONY, ALPHA, P, BETA);
	antColony.SetSameCounter(20);
	double time = timer.GetTime("all");
	std::cout << "Czas: " << time;
	graph.Release();

	std::vector<std::vector<unsigned int>> result = antColony.GetBestPaths();
	std::cout << std::endl;
	for (int i = 0; i < result.size(); i++)
	{
		for (int j = 0; j < result[i].size(); j++)
		{
			std::cout << result[i][j] << " ";
		}
		std::cout << std::endl;
	}
	system("pause");
}

