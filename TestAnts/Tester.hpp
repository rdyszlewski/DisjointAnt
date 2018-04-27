#pragma once
#include <vector>
#include "../Common/WriteListener.hpp"
#include "Logger.hpp"
#include "ResultWriter.hpp"

typedef std::vector<std::string> NamesVector;
typedef std::vector<std::vector<double>> ValuesVector;

class ResultListener : public WriteListener
{
public:
	ResultListener(Logger& logger, ResultWriter& writer) :m_logger(logger), m_writer(writer) {}
	void WriteResult(int iteration, std::vector<int> values)
	{
		m_writer.WriteResult(iteration, values);
	};
	void WriteLog(std::string message)
	{
		m_logger.WriteLog(message);
	};

private:
	Logger & m_logger;
	ResultWriter& m_writer;
};

class Tester
{
public:
	Tester();
	~Tester();

	void Init(NamesVector parametersNames, ValuesVector parametersValues, int numberIteration, int numberSameObjectiveFunction);
	void Start(std::string resultPath, std::string resultName);

private:
	std::vector<std::string> CreateHeader(NamesVector parametersNames, ValuesVector parametersValues, std::vector<int> counters);
	void Iterate(std::vector<int>& counters, int k);
	void StartAlgorithm();
private:
	NamesVector m_parameters_names;
	ValuesVector m_parameters_values;
	ResultListener* m_result_listener;
	int m_iteration;
	int m_same_objective_function;
};

