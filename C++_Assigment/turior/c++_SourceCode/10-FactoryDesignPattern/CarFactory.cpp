#include "CarFactory.h"
#include "Sedan.h"
#include "SUV.h"

Car* CarFactory::createCar(const string& type)
{
	if (type == "Sedan")
	{
		return new Sedan();
	}
	else if (type == "SUV")
	{
		return new SUV();
	}
	else
	{
		return nullptr;
	}
}

