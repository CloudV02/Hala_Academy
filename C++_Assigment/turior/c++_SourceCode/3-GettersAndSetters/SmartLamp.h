#ifndef SMARTLAMP_H
#define SMARTLAMP_H

#include <string>
using namespace std;

class SmartLamp
{
private:
	string model;
	int brightness;

public:
	// Constructor to initialize the lamp
	SmartLamp(string lampModel);

	// Getter for brightness
	int getBrightness() const;

	// Setter for brightness
	void setBrightness(int level);

	// Method to display lamp status
	void displayStatus() const;
};

#endif
