#pragma once

#include <iostream>
#include <fstream>
#include <vector>

class Logger
{
public:
	Logger();
	~Logger();

	void OpenLogFile(const std::string path);
	void WriteLog(const std::string message);
	void CloseLogFile();

private:
	std::string GetTime();
private:
	std::ofstream m_log_file;
};

