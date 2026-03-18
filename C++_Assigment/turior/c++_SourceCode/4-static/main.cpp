#include "Worker.h"
#include "Manager.h"
#include <iostream>
using namespace std;

int main()
{
	Worker w1("Alice", 50000);
	Worker w2("Bob", 45000);

	Manager mgr;
	mgr.printWorkerInfo(w1);
	mgr.printWorkerInfo(w2);

	double total = mgr.getTotalSalary(w1, w2);
	cout << "Total Salary: " << total << endl;

	return 0;
}
