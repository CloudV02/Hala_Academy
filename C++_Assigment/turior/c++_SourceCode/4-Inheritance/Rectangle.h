#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"
#include <string>
using namespace std;

class Rectangle : public Shape
{
	private:
		double length, width;

	public:
		Rectangle(string c, double l, double w); // Constructor

		// Method specific to Rectangle, not shared with Circle
		void calculatePerimeter() const;

		// Display specific Rectangle info
		void displayRectangleDetails() const;
};

#endif
