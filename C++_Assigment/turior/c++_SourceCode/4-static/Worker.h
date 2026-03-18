#ifndef WORKER_H
#define WORKER_H

#include <string>
using namespace std;

class Worker
{
	private:
		string name;
		double salary;

	public:
		Worker(const string& name, double salary);

		// Give Manager access to private members
		friend class Manager;
};

#endif
