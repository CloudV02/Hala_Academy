#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"
#include <string>
using namespace std;

class Circle : public Shape
{
	private:
		double radius;

	public:
		Circle(string c, double r); // Constructor

		// Method specific to Circle, not shared with Rectangle
		void calculateArea() const;

		// Display specific Circle info
		void displayCircleDetails() const;
};

#endif
