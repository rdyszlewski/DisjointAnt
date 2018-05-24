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
	std::srand(time(NULL));
	Graph graph = Graph();
	//int data[] = { 0,0,3,0,0,0,0,1,0,0,0,0,0,4,1,0,0,0,0,0,0,0,0,0,0 };
	//int source[] = { 0,1 };
	//int target[] = { 3,4 };

	//int data[] = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 6, 12, 0, 0, 0, 0, 0, 4, 5, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	//int source[] = { 0, 1 };
	//int target[] = { 5, 6 };
	int data[] = { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int source[] = { 2, 1, 0 };
	int target[] = { 9, 10, 8 };
	Logger logger;
	logger.OpenLogFile("D://log.txt");
	ResultWriter writer;
	writer.OpenResultFile("D://", "wynik.csv", ';');
	Listener* listener =new  Listener(logger, writer);
	WriteListener* writeListener = listener;
	graph.Init(data, 11, source, target, 3);
	AntColony antColony(50);
	antColony.SetWriteListener(writeListener);
	Timer timer;
	timer.Start("all");
	antColony.Start(&graph, 50, 1, 1, 0.75, 1);
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

