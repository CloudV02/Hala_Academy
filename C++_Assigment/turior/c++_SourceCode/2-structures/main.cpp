#include <iostream>
using namespace std;

// Define a structure
struct Student
{
    string name;
    int age;
    float grade;
};

int main()
{
    // Create a variable of type Student
    Student s1;

    // Assign values
    s1.name = "Alice";
    s1.age = 20;
    s1.grade = 88.5;

    // Print the student info
    cout << "Name: " << s1.name << endl;
    cout << "Age: " << s1.age << endl;
    cout << "Grade: " << s1.grade << endl;

    Student s2;

    s2.name = "John";
    s2.age = 21;
    s2.grade = 75.2;

    cout << "Name: " << s2.name << endl;
	cout << "Age: " << s2.age << endl;
	cout << "Grade: " << s2.grade << endl;

    return 0;
}
