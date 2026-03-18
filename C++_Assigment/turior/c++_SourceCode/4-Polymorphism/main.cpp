#include "Car.h"
#include "Truck.h"

int main()
{
	//Option 1 : Car and Truck objects
	Car car1("Toyota", 2018, 4);
	Truck truck1("Ford", 2020, 15);

	car1.displayInfo();
	truck1.displayInfo();

	//Option 2 : pointer of Vehicle
	Vehicle* vehicles[2];

	vehicles[0] = new Car("Toyota", 2020, 4);
	vehicles[1] = new Truck("Ford", 2018, 3.5);

	for (int i = 0; i < 2; i++)
	{
		vehicles[i]->displayInfo(); // Polymorphism: calls the correct displayInfo method
	}

	// Cleanup
	delete vehicles[0];
	delete vehicles[1];
}
