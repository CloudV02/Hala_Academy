#include "WeatherStation.h"
#include <algorithm>
using namespace std;

void WeatherStation::addObserver(Observer* observer)
{
	observers.push_back(observer);
}

void WeatherStation::removeObserver(Observer* observer)
{
	observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
}

void WeatherStation::setTemperature(float newTemperature)
{
	temperature = newTemperature;
	notifyObservers();
}

void WeatherStation::notifyObservers()
{
	for (Observer* observer : observers)
	{
		observer->update(temperature);
	}
}
