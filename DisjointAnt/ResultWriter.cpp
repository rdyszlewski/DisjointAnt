#include "ResultWriter.hpp"
#include <sstream>
#include <cassert>

ResultWriter::ResultWriter()
{
}


ResultWriter::~ResultWriter()
{
}

void ResultWriter::OpenResultFile(const std::string path, const std::string filename, char separator)
{
	m_separator = separator;
	if (m_result_file.is_open())
	{
		CloseResultFile();
	}

	std::string fullPath = path + "/" + filename;
	m_result_file.open(fullPath, std::ios_base::app);
}

void ResultWriter::WriteHeader(std::vector<std::string>& headers)
{
	assert(m_result_file.is_open());
	WriteVector(headers, m_result_file, m_separator);
}

void ResultWriter::WriteResult(int iteration, std::vector<int>& results)
{
	assert(m_result_file.is_open());
	assert(!results.empty());
	m_result_file << iteration <<m_separator;
	WriteVector(results,m_result_file, m_separator);
}

template<typename T>
void ResultWriter::WriteVector(std::vector<T>& vector, std::ofstream& resultFile, char separator)
{
	int size = vector.size();
	for (int i = 0; i < size; i++)
	{
		resultFile << vector[i];
		if (i != size - 1)
		{
			resultFile << separator;
		}
	}
	resultFile << std::endl;
}

void ResultWriter::CloseResultFile()
{
	assert(m_result_file.is_open());
	m_result_file.close();
}