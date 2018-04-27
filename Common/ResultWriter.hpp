#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class ResultWriter
{
public:
	ResultWriter();
	~ResultWriter();

	void OpenResultFile(const std::string path, const std::string filename, char separator);
	void WriteHeader(std::vector<std::string>& headers);
	void WriteResult(int iteration, std::vector<int>& results);
	void CloseResultFile();

private:
	template<typename T>
	void WriteVector ( std::vector<T>& vector, std::ofstream& resultFile, char separator);
private:
	std::ofstream m_result_file;
	char m_separator;
};

