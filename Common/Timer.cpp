#include "Timer.hpp"
#include <iostream>


Timer::Timer()
{
}


Timer::~Timer()
{
}

void Timer::Start(std::string tag)
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
	{
		std::cout << "QueryPerformanceFrequency Failed \n";
	}
	m_pc_freq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	m_starts[tag] = li.QuadPart;
}

double Timer::GetTime(std::string tag)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - m_starts[tag]) / m_pc_freq;
}
