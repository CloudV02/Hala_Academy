#include "Circle.h"
#include <iostream>

Circle::Circle(string c, double r) : Shape(c), radius(r) {}

void Circle::calculateArea() const
{
	double area = 3.14 * radius * radius;
	cout << "Area of the circle: " << area << endl;
}

void Circle::displayCircleDetails() const
{
	displayColor();
	cout << "Circle radius: " << radius << endl;
}
