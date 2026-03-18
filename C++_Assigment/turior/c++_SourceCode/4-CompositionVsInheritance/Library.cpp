#include "Library.h"

void Library::addBook(const Book& book)
{
	for(unsigned int i = 0; i < 100; i++)
	{
		if(books[i].getTitle() == "")
		{
			books[i] = book;
			break;
		}
	}
}

void Library::displayBooks() const
{
	for(unsigned int i = 0; i < 100; i++)
	{
		if(books[i].getTitle() != "")
		{
			books[i].displayInfo(); // Call Book's displayInfo
		}
		else
		{
			break;
		}
	}
}
