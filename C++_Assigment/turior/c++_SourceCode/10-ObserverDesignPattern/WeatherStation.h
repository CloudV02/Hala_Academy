#ifndef WEATHERSTATION_H
#define WEATHERSTATION_H

#include "Observer.h"
#include <vector>
using namespace std;

class WeatherStation
{
	private:
		float temperature;
		vector<Observer*> observers;

	public:
		void addObserver(Observer* observer);
		void removeObserver(Observer* observer);
		void setTemperature(float newTemperature);
		void notifyObservers();
};

#endif
