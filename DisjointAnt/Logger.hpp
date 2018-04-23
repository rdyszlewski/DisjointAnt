#pragma once

#include <iostream>
#include <fstream>

class Logger
{
public:
	Logger();
	~Logger();

	void OpenResultFile(const std::string& file);
	void OpenLogFile(const std::string& file);
	void CloseResultFile();
	void CloseLogFile();
	//TODO powstawiaæ odpowiednie parametry
	void StartNewResult();
	void SaveResult(int step, int result);
	void SaveLog(std::string log);

private:
	std::ofstream m_result_file;
	std::ofstream m_log_file;
};

