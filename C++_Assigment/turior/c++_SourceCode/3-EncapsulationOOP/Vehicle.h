#ifndef VEHICLE_H // ifndef = if not defined
#define VEHICLE_H

#include <string>
using namespace std;

class Vehicle
{
	private: //Encapsulation of attributes with private access modifier
		string model;
		int speed;

	public:
		// Constructor
		Vehicle(string vehicleModel);

		// Getter for speed
		int getSpeed() const;

		// Setter for speed
		void setSpeed(int newSpeed);

		// Method to display vehicle info
		void displayInfo() const;
};

#endif
