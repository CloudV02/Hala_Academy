#ifndef SEDAN_H
#define SEDAN_H

#include "Car.h"

class Sedan : public Car
{
public:
	void display() const override;
};

#endif
