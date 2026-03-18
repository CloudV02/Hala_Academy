#include "WeatherStation.h"
#include "Display.h"

int main()
{
	WeatherStation station;
	Display display;

	station.addObserver(&display);

	station.setTemperature(25.5f);
	station.setTemperature(30.0f);

	return 0;
}
