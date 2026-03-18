#include "Employee.h"
#include <fstream>
#include <iostream>

Employee::Employee() : name(""), id(0), salary(0.0) {}

Employee::Employee(const string& name, int id, double salary)
    : name(name), id(id), salary(salary) {}

void Employee::serialize(const string& filename) const
{
    ofstream outFile(filename, ios::binary);
    if (!outFile)
    {
        cerr << "Failed to open file for writing." << endl;
        return;
    }

    size_t nameLen = name.size();
    outFile.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
    outFile.write(name.c_str(), nameLen);
    outFile.write(reinterpret_cast<const char*>(&id), sizeof(id));
    outFile.write(reinterpret_cast<const char*>(&salary), sizeof(salary));

    outFile.close();
}

void Employee::deserialize(const string& filename)
{
    ifstream inFile(filename, ios::binary);
    if (!inFile)
    {
        cerr << "Failed to open file for reading." << endl;
        return;
    }

    size_t nameLen;
    inFile.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
    name.resize(nameLen);
    inFile.read(&name[0], nameLen);
    inFile.read(reinterpret_cast<char*>(&id), sizeof(id));
    inFile.read(reinterpret_cast<char*>(&salary), sizeof(salary));

    inFile.close();
}

void Employee::display() const
{
    cout << "Name: " << name << "\nID: " << id << "\nSalary: " << salary << endl;
}
