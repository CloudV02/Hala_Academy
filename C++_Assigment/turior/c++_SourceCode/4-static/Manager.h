#ifndef MANAGER_H
#define MANAGER_H

#include "Worker.h"

class Manager
{
	public:
		void printWorkerInfo(const Worker& worker) const;
		double getTotalSalary(const Worker& w1, const Worker& w2) const;
};

#endif
