#pragma once

#include <map>
#include <windows.h>

class Timer
{
public:
	Timer();
	~Timer();

	void Start(std::string tag);
	//zwraca czas wykonywania metody w milisekundach(chyba)
	double GetTime(std::string tag);
	
private:
	std::map<std::string, __int64> m_starts;
	double m_pc_freq;
};

