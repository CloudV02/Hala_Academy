#ifndef CARFACTORY_H
#define CARFACTORY_H

#include "Car.h"
#include <string>
using namespace std;

class CarFactory
{
	public:
		static Car* createCar(const string& type);
};

#endif
