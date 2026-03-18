#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"

class Library
{
	private:
		Book books[100]; // Composition: Library "has-a" Book

	public:
		void addBook(const Book& book); // Method to add books
		void displayBooks() const; // Method to display books
};

#endif
