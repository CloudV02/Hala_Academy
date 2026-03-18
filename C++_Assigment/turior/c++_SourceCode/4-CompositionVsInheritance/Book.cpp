#include "Book.h"
#include <iostream>
using namespace std;

Book::Book() : title(""), author("") {}

Book::Book(string t, string a) : title(t), author(a) {}

string Book::getTitle() const
{
	return title;
}

void Book::displayInfo() const
{
	cout << "Title: " << title << ", Author: " << author << endl;
}
