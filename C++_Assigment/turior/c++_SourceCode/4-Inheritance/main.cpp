#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"

int main()
{
	// Create objects of Circle and Rectangle
	Circle myCircle("Red", 5);
	Rectangle myRectangle("Blue", 4, 6);

	// Use the display and specific methods for each shape
	myCircle.displayCircleDetails(); // Circle-specific details
	myRectangle.displayRectangleDetails(); // Rectangle-specific details

	// Call specific methods for calculations
	myCircle.calculateArea(); // Area of circle
	myRectangle.calculatePerimeter(); // Perimeter of rectangle
}
