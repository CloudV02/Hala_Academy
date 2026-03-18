#include "SmartDevice.h"
#include <iostream>
using namespace std;

SmartDevice::SmartDevice() : name("Default Smart Device"), isOn(false)
{
	cout << name << " device created." << endl;
}

SmartDevice::SmartDevice(string deviceName) : name(deviceName), isOn(false)
{
	cout << name << " device created." << endl;
}

SmartDevice::~SmartDevice()
{
	cout << name << " device destroyed." << endl;
}

void SmartDevice::turnOn()
{
	isOn = true;
	cout << name << " is now ON." << endl;
}

void SmartDevice::displayStatus() const
{
	cout << "Device: " << name << ", Status: " << (isOn ? "ON" : "OFF") << endl;
}
