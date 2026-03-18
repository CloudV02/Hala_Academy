#include <iostream>
#include "Employee.h"
using namespace std;

int main()
{
    Employee e1("Alice Johnson", 101, 75000.0);

    // Serialize to file
    e1.serialize("employee.dat");

    // Deserialize into a new object
    Employee loaded;
    loaded.deserialize("employee.dat");

    cout << "Deserialized Employee:\n";
    loaded.display();

    return 0;
}
