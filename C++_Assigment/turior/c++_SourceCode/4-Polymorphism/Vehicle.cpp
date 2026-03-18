#include "Vehicle.h"

Vehicle::Vehicle() : brand("Unknown"), year(2025) {}

Vehicle::Vehicle(string b, int y) : brand(b), year(y) {}

void Vehicle::displayInfo() const
{
	cout << "Brand: " << brand << ", Year: " << year << endl;
}

Vehicle::~Vehicle() {}
