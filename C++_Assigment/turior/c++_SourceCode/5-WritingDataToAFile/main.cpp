#include <fstream>
#include <iostream>
using namespace std;

int main()
{
	ofstream outFile("example.txt");
	if (outFile.is_open())
	{
		outFile << "This is a simple file write example." << endl;
		outFile << "This is a second simple file write example." << endl;
		outFile.close();
	}
	else
	{
		cerr << "Error opening file for writing." << endl;
	}
	return 0;
}
