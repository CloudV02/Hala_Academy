#include "LibraryMember.h"
#include <iostream>

LibraryMember::LibraryMember(string name, string id) : Person(name), memberId(id) {}

void LibraryMember::displayInfo() const
{
	Person::displayInfo(); // Call base class method
	cout << "Member ID: " << memberId << endl;
}
