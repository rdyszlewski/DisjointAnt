#include "Tester.hpp"
#include "AntColony.hpp"
#include <sstream>
#include <iostream>

Tester::Tester()
{
}


Tester::~Tester()
{
}

void Tester::Init(NamesVector parametersNames, ValuesVector paramatersValues, int numberIteration, int numberSameObjectiveFunction)
{
	m_parameters_names = parametersNames;
	m_parameters_values = paramatersValues;
	m_iteration = numberIteration;
	m_same_objective_function = numberSameObjectiveFunction;
}

void Tester::Start(std::string resultPath, std::string resultName)
{
	Logger logger;
	logger.OpenLogFile("D://log.txt");
	ResultWriter writer;
	int resultNumber = 0;
	m_result_listener = new ResultListener(logger, writer);
	WriteListener* writerListener = m_result_listener;
	AntColony antColony(0);
	antColony.SetWriteListener(writerListener);

	std::vector<int> counters;
	int parametersSize = m_parameters_names.size();
	for (int i = 0; i < parametersSize; i++)
	{
		counters.push_back(0);
	}
	int testCase = 0;
	Iterate(counters, 0);
	system("pause");
}

void Tester::Iterate(std::vector<int>& counters, int k)
{
	for (int i = 0; i < m_parameters_values[k].size(); i++)
	{
		counters[k] = i;
		
		if (k != m_parameters_values.size() - 1)
		{
			Iterate(counters, k + 1);
		}
		else
		{
			StartAlgorithm();
			std::vector<std::string> headers = CreateHeader(m_parameters_names, m_parameters_values, counters);
			for (std::string s : headers)
			{
				std::cout << s << " ";
			}
			std::cout << std::endl;
		}
	}
}

void Tester::StartAlgorithm()
{
	//TODO wystartowaæ algorytm
}

std::vector<std::string> Tester::CreateHeader(NamesVector parametersNames, ValuesVector parametersValues, std::vector<int> counters)
{
	std::vector<std::string> headers;
	int parametersSize = parametersNames.size();
	for (int i = 0; i < parametersSize; i++)
	{ 
		std::ostringstream text;
		text << parametersNames[i] << "=" << parametersValues[i][counters[i]];
		headers.push_back(text.str());
	}
	return std::move(headers);
}