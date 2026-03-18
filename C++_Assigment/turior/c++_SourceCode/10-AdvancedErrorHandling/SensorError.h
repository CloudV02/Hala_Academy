#ifndef SENSORERROR_H
#define SENSORERROR_H

#include <exception>
#include <string>
using namespace std;

class SensorError : public exception
{
	private:
		string msg_;

	public:
		explicit SensorError(const string& msg);
		virtual const char* what() const noexcept override;
};

class SensorNotInitialized : public SensorError
{
	public:
		SensorNotInitialized();
};

class SensorReadFailure : public SensorError
{
	public:
		SensorReadFailure();
};

#endif
