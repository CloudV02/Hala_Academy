#include "Rectangle.h"
#include <iostream>

Rectangle::Rectangle(string c, double l, double w) : Shape(c), length(l), width(w) {}

void Rectangle::calculatePerimeter() const
{
	double perimeter = 2 * (length + width);
	cout << "Perimeter of the rectangle: " << perimeter << endl;
}

void Rectangle::displayRectangleDetails() const
{
	displayColor();
	cout << "Rectangle dimensions: " << length << " x " << width << endl;
}
