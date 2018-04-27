#include "Tester.hpp"

int main()
{
	Tester tester;
	std::vector<std::string> names = { "alpha", "beta", "p" };
	std::vector<std::vector<double>> values;
	std::vector<double> alpha = { 1,2,3 };
	std::vector<double> beta = { 4, 5 };
	std::vector<double> p = { 0, 1 };
	values.push_back(alpha);
	values.push_back(beta);
	values.push_back(p);
	tester.Init(names, values, 10, 5);
	tester.Start("D://", "tester");
}