#include "Graph.hpp"
#include "AntColony.hpp"
#include "Logger.hpp"
#include "ResultWriter.hpp"
#include "WriteListener.hpp"

class Listener : public WriteListener 
{
public:
	Listener(Logger& logger, ResultWriter& writer) :m_logger(logger), m_writer(writer) {}
	void writeResult(int iteration, std::vector<int> values)
	{
		m_writer.WriteResult(iteration, values);
	};
	void writeLog(std::string message)
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
	Graph graph = Graph();
	int data[] = { 0,0,3,0,0,0,0,1,0,0,0,0,0,4,1,0,0,0,0,0,0,0,0,0,0 };
	int source[] = { 0,1 };
	int target[] = { 3,4 };
	Logger logger;
	logger.OpenLogFile("D://log.txt");
	ResultWriter writer;
	writer.OpenResultFile("D://", "wynik.csv", ';');
	Listener* listener =new  Listener(logger, writer);
	WriteListener* writeListener = listener;
	graph.Init(data, 5, source, target, 2);
	AntColony antColony(10);
	antColony.SetWriteListener(writeListener);
	antColony.Start(&graph, 10, 1, 1, 0.1, 1);
	
	graph.Release();
	system("pause");
}

