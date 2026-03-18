#include <fstream>
#include <iostream>
using namespace std;

int main()
{
	fstream file("example.txt", ios::out | ios::app); // Open file in append mode
	if (file.is_open())
	{
		file << "Appending new line to the file." << endl;
		file.close();
	}
	else
	{
		cerr << "Error opening file for appending." << endl;
	}
	return 0;
}
