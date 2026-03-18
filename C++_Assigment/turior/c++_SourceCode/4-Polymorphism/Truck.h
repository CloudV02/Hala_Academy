#ifndef TRUCK_H
#define TRUCK_H

#include "Vehicle.h"

class Truck : public Vehicle
{
	private:
		double loadCapacity;

	public:
		Truck(string b, int y, double capacity); // Constructor

		// Overridden method for Truck
		void displayInfo() const override;
};

#endif
