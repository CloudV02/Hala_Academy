#ifndef SMARTLIGHT_H
#define SMARTLIGHT_H

#include <string>
using namespace std;

class SmartLight
{
private:
	string model;
	bool isOn;

public:
	// Constructor
	SmartLight(string lightModel);

	// Method to turn on the light using the 'this' pointer
	void turnOn();

	// Method to display the status
	void displayStatus() const;
};

#endif
