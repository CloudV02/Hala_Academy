#include "Person.h"
#include <iostream>
using namespace std;

Person::Person(string n) : name(n) {}

void Person::displayInfo() const
{
	cout << "Name: " << name << endl;
}
