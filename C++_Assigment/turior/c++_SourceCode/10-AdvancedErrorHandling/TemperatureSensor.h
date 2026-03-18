#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

class TemperatureSensor
{
	private:
		bool initialized;
		int readCount;

	public:
		TemperatureSensor();
		void initialize();
		double readTemperature();
};

#endif
