#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
using namespace std;

class Employee
{
	private:
		string name;
		int id;
		double salary;

	public:
		Employee();
		Employee(const string& name, int id, double salary);

		void serialize(const string& filename) const;
		void deserialize(const string& filename);

		void display() const;
};

#endif
