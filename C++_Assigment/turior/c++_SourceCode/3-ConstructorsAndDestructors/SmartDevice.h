#ifndef SMARTDEVICE_H
#define SMARTDEVICE_H

#include <string>
using namespace std;

class SmartDevice
{
	private:
		string name;
		bool isOn;

	public:
		//default constructor
		SmartDevice();
		// Constructor to initialize the device
		SmartDevice(string deviceName);

		// Destructor to display device cleanup
		~SmartDevice();

		// Method to turn the device on
		void turnOn();

		// Method to display device status
		void displayStatus() const;
};

#endif
