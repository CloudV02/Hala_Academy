#include "Shape.h"
#include <iostream>

Shape::Shape(string c) : color(c) {}

void Shape::displayColor() const
{
	cout << "Shape color: " << color << endl;
}

Shape::~Shape() {}
