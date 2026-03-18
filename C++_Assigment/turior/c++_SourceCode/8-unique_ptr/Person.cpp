#include "Person.h"
#include <iostream>

Person::Person(const string& name) : name(name) {}

void Person::greet() const
{
	cout << "Hello, " << name << "!" << endl;
}
