#include <fstream>
#include <iostream>
#include <string>
using namespace std;

struct Student
{
	string name;
	int age;
};

int main()
{
	Student s1 = {"John Doe", 21};

	//Writing from a binary file
	fstream file("student.dat", ios::out | ios::binary);
	if (file.is_open())
	{
		file.write(reinterpret_cast<const char*>(&s1), sizeof(s1)); // Writing binary data
		file.close();
	}
	else
	{
		cerr << "Error opening binary file for writing." << endl;
	}

	//Reading from a binary file
	fstream file2("student.dat", ios::in | ios::binary);
	if (file2.is_open())
	{
		file2.read(reinterpret_cast<char*>(&s1), sizeof(s1)); // Reading binary data
		cout << "Name: " << s1.name << ", Age: " << s1.age << endl;
		file2.close();
	}
	else
	{
		cerr << "Error opening binary file for reading." << endl;
	}

	return 0;
}
