#include <fstream>
#include <iostream>
#include <string>
using namespace std;

int main() {
	ifstream inFile("example.txt");
	string line;
	if (inFile.is_open())
	{
		while (getline(inFile, line))
		{
			cout << line << endl;
		}
		inFile.close();
	}
	else
	{
		cerr << "Error opening file for reading." << endl;
	}
	return 0;
}
