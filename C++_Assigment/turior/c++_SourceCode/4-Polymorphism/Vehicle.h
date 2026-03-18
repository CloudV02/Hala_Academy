#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <iostream>
using namespace std;

class Vehicle
{
	protected:
		string brand;
		int year;

	public:
		Vehicle();
		Vehicle(string b, int y); // Constructor

		// Common method for all vehicles
		virtual void displayInfo() const;

		virtual ~Vehicle(); // Virtual destructor
};

#endif
