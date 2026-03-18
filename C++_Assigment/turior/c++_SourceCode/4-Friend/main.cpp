#include <iostream>
#include "Counter.h"
using namespace std;

int main()
{
    // Call STATIC function WITHOUT creating any objects
    cout << "Before creating objects: ";
    Counter::showStatistics();

    // Create objects
    Counter c1(10);
    Counter c2(20);
    Counter c3(30);

    // Call STATIC function - belongs to CLASS, not objects
    cout << "Total counters: " << Counter::getTotalCounters() << endl;

    c1.increment();
    cout << "c1 value: " << c1.getValue() << endl;

    return 0;
}
