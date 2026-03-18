#ifndef COMPLEXNUMBER_H
#define COMPLEXNUMBER_H

#include <iostream>
using namespace std;

class ComplexNumber
{
	private:
		double real, imag;

	public:
		ComplexNumber(double r, double i); // Constructor

		//operators overloading
		ComplexNumber operator+(const ComplexNumber& other) const;
		bool operator==(const ComplexNumber& other) const;
		friend ostream& operator<<(ostream& out, const ComplexNumber& c);

		// Method to display complex number
		void display() const;
};

#endif
