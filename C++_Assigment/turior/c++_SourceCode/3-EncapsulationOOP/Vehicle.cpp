#include "Vehicle.h"
#include <iostream>
using namespace std;

Vehicle::Vehicle(string vehicleModel)
{
	model = vehicleModel;
	speed = 0; // Default speed
}

int Vehicle::getSpeed() const
{
	return speed;
}

void Vehicle::setSpeed(int newSpeed)
{
	if (newSpeed >= 0)
	{
		speed = newSpeed;
	}
	else
	{
		cout << "Speed cannot be negative!" << endl;
	}
}

void Vehicle::displayInfo() const
{
	cout << "Vehicle Model: " << model << ", Speed: " << speed << " km/h" << endl;
}
