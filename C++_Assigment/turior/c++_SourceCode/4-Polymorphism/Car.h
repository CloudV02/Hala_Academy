#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"

class Car : public Vehicle
{
	private:
		int doors;

	public:
		Car(string b, int y, int d); // Constructor

		// Overridden method for Car
		void displayInfo() const override;
};

#endif
