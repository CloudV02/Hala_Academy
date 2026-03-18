// Counter.cpp - Implementation
#include "Counter.h"

int Counter::totalCounters = 0;

Counter::Counter(int val) : value(val)
{
    totalCounters++;  // Increment shared counter
    cout << "Counter created. Total counters: " << totalCounters << endl;
}

Counter::~Counter()
{
    totalCounters--;
    cout << "Counter destroyed. Remaining: " << totalCounters << endl;
}

void Counter::increment()
{
    value++;
}

int Counter::getValue() const
{
    return value;
}

// STATIC functions - can access only static members
int Counter::getTotalCounters()
{
    return totalCounters;
    //return value;  // ERROR! Can't access non-static members
}

void Counter::showStatistics()
{
    cout << "=== STATIC INFO ===" << endl;
    cout << "Total Counter objects: " << totalCounters << endl;
    cout << "===================" << endl;
}
