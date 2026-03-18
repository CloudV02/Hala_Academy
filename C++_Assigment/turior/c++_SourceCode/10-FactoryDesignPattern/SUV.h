#ifndef SUV_H
#define SUV_H

#include "Car.h"

class SUV : public Car
{
public:
	void display() const override;
};

#endif
