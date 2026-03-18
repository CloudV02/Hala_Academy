#include "CarFactory.h"
#include "Car.h"
#include "Sedan.h"


int main()
{
	Car* myCar = CarFactory::createCar("Sedan");
	myCar->display();
	delete myCar;

	myCar = CarFactory::createCar("SUV");
	myCar->display();
	delete myCar;

	return 0;
}
