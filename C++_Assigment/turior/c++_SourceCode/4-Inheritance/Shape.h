#ifndef SHAPE_H
#define SHAPE_H

#include <string>
using namespace std;

class Shape
{
	protected:
		string color;

	public:
		Shape(string c); // Constructor

		// Display basic shape info (shared functionality)
		void displayColor() const;

		// Virtual destructor for proper cleanup
		virtual ~Shape();
};

#endif
