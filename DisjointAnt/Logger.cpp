#include "Logger.hpp"
#include <iomanip>
#include <ctime>
#include <sstream>

Logger::Logger()
{
}


Logger::~Logger()
{
}

void Logger::OpenLogFile(const std::string path)
{
	if (m_log_file.is_open())
	{
		CloseLogFile();
	}

	m_log_file.open(path, std::ios_base::app);
}

void Logger::WriteLog(const std::string message)
{
	//TOOD mo¿na dodaæ datê 
	std::string time = GetTime();
	m_log_file << time << "\t" << message << std::endl;
}

std::string Logger::GetTime()
{
	/*auto t = std::time(nullptr);
	#define _CRT_SECURE_NO_WARNINGS
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	return oss.str();*/
	
	//TODO zrobiæ zwracanie czasu
	return "";
}

void Logger::CloseLogFile()
{
	m_log_file.close();
 }