#include <iostream>
using namespace std;

int main()
{
	struct Student
	{
	    string name;
	    int age;
	    float grade;
	};

    // Create an array of 3 students
    Student students[3];

    // Input data
    for (int i = 0; i < 3; i++)
    {
        cout << "Enter info for student " << i + 1 << ":\n";
        cout << "Name: ";
        cin >> students[i].name;
        cout << "Age: ";
        cin >> students[i].age;
        cout << "Grade: ";
        cin >> students[i].grade;
    }

    // Display all students
    cout << "\nStudent List:\n";
    for (int i = 0; i < 3; i++)
    {
        cout << "Name: " << students[i].name << ", Age: " << students[i].age
             << ", Grade: " << students[i].grade << endl;
    }

    return 0;
}
