#include "SmartDevice.h"

int main()
{
	SmartDevice s1("Home");
	s1.displayStatus();
	s1.turnOn();
	s1.displayStatus();

	SmartDevice s2("WarHouse");
	s2.displayStatus();

	SmartDevice s3("Pool");
	s3.displayStatus();
	s3.turnOn();
	s3.displayStatus();

	SmartDevice s4;
	s4.displayStatus();

	return 0;
}
