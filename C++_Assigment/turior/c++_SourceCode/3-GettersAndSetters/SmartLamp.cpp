#include "SmartLamp.h"
#include <iostream>
using namespace std;

SmartLamp::SmartLamp(string lampModel)
{
	model = lampModel;
	brightness = 0; // Default brightness level is 0
}

int SmartLamp::getBrightness() const
{
	return brightness;
}

void SmartLamp::setBrightness(int level)
{
	if (level >= 0 && level <= 100)
	{
		brightness = level;
	}
	else
	{
		cout << "Invalid brightness level!" << endl;
	}
}

void SmartLamp::displayStatus() const
{
	cout << "Lamp Model: " << model << ", Brightness: " << brightness << "%" << endl;
}
