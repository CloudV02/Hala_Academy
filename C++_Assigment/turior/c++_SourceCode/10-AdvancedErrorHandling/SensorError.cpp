#include "SensorError.h"
using namespace std;

SensorError::SensorError(const string& msg) : msg_(msg) {}

const char* SensorError::what() const noexcept
{
    return msg_.c_str();
}

SensorNotInitialized::SensorNotInitialized()
    : SensorError("Sensor has not been initialized.") {}

SensorReadFailure::SensorReadFailure()
    : SensorError("Failed to read from the sensor.") {}
