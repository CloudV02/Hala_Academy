#ifndef PERSON_H
#define PERSON_H

#include <string>
using namespace std;

class Person
{
	private:
		string name;

	public:
		Person(string n); // Constructor
		virtual void displayInfo() const; // Virtual function for polymorphism
		virtual ~Person() = default;
};

#endif
