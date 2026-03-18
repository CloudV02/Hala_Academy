#include "Car.h"

Car::Car(string b, int y, int d) : Vehicle(b, y), doors(d) {}

void Car::displayInfo() const
{
	Vehicle::displayInfo(); // Call base class method
	cout << "Doors: " << doors << endl;
}
