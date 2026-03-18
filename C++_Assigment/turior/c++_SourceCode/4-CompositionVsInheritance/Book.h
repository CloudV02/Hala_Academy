#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

class Book
{
	private:
		string title;
		string author;

	public:
		Book();
		Book(string t, string a); // Constructor
		string getTitle() const;
		void displayInfo() const; // Method to display book info
};

#endif
