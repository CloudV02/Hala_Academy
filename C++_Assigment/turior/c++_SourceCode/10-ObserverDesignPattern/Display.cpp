#include "Display.h"
#include <iostream>
using namespace std;

void Display::update(float temperature)
{
	cout << "Temperature updated to: " << temperature << "°C" << endl;
}
