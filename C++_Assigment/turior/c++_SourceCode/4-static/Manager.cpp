#include "Manager.h"
#include <iostream>
using namespace std;

void Manager::printWorkerInfo(const Worker& worker) const
{
	cout << "Worker Name: " << worker.name << ", Salary: " << worker.salary <<	endl;
}

double Manager::getTotalSalary(const Worker& w1, const Worker& w2) const
{
	return w1.salary + w2.salary;
}
