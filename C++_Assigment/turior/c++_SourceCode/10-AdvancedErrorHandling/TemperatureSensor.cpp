#include "TemperatureSensor.h"
#include "SensorError.h"

TemperatureSensor::TemperatureSensor()
    : initialized(false), readCount(0) {}

void TemperatureSensor::initialize()
{
    initialized = true;
    readCount = 0;
}

double TemperatureSensor::readTemperature()
{
    if (!initialized)
    {
        throw SensorNotInitialized();
    }

    ++readCount;

    // Fail every 3rd read
    if (readCount % 3 == 0)
    {
        throw SensorReadFailure();
    }

    // Return a gradually increasing temperature
    return 25.0 + (readCount * 0.5); // e.g., 25.5, 26.0, 27.0...
}
