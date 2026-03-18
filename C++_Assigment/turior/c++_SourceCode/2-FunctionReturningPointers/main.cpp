#include <iostream>
using namespace std;

int* badFunction()
{
    int x = 10;
    return &x;  // ❌ ERROR: x is destroyed when the function ends
}

int* createNumber()
{
    int* ptr = new int;  // allocate memory on the heap
    *ptr = 42;           // assign value
    return ptr;          // return pointer to it
}

int* createArray(int size)
{
    int* arr = new int[size];  // dynamic array
    for (int i = 0; i < size; ++i)
        arr[i] = i * 10;
    return arr;
}

struct Student
{
    string name;
    int age;
};

Student* createStudent()
{
    Student* s = new Student;
    s->name = "Alice";
    s->age = 21;
    return s;
}

int main()
{
	//int* ptr2 = badFunction(); //Not working due to local variable in badFunction
								 //that doesn't exist anymore when this line is executed
	//cout << "Value at the address of ptr2" << *ptr2 << endl;
    int* num = createNumber();   // get pointer
    cout << "Value: " << *num << endl;  // prints 42
    delete num;  // don't forget to free the memory!

    int size = 5;
	int* numbers = createArray(size);

	for (int i = 0; i < size; ++i)
		cout << numbers[i] << " ";
	cout << endl;

	delete[] numbers;  // clean up

	Student* ptr = createStudent();
	cout << "Name: " << ptr->name << ", Age: " << ptr->age << endl;
	delete ptr;

    return 0;
}
