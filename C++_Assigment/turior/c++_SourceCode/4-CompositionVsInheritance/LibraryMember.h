#ifndef LIBRARYMEMBER_H
#define LIBRARYMEMBER_H

#include "Person.h"


class LibraryMember : public Person
{
	private:
		string memberId;

	public:
		LibraryMember(string name, string id); // Constructor
		void displayInfo() const override; // Overriding Person's method
};

#endif
