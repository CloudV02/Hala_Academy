#include "SmartLight.h"
#include <iostream>
using namespace std;

SmartLight::SmartLight(string lightModel)
{
	model = lightModel;
	isOn = false;
}

void SmartLight::turnOn()
{
	this->isOn = true; // Using 'this' pointer to refer to the current object
	cout << model << " light is now ON." << endl;
}

void SmartLight::displayStatus() const
{
	cout << "Smart Light Model: " << model << ", Status: " << (isOn ? "ON" : "OFF") << endl;
}
