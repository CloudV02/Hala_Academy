#include "Truck.h"

Truck::Truck(string b, int y, double capacity) : Vehicle(b, y), loadCapacity(capacity) {}

void Truck::displayInfo() const
{
	Vehicle::displayInfo(); // Call base class method
	cout << "Load Capacity: " << loadCapacity << " tons" << endl;
}
