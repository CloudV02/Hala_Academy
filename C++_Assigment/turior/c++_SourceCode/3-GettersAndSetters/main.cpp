#include "SmartLamp.h"
#include <iostream>
using namespace std;

int main()
{
	SmartLamp s1("Blue Lamp");
	cout << s1.getBrightness() << endl;
	s1.displayStatus();

	s1.setBrightness(65);
	s1.displayStatus();

	return 0;
}
