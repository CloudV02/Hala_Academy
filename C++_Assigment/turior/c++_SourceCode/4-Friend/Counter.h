// Counter.h - Demonstrates STATIC members
#ifndef COUNTER_H
#define COUNTER_H

#include <iostream>
using namespace std;

class Counter
{
	private:
		int value;
		static int totalCounters;  // STATIC - shared by all objects

	public:
		Counter(int val = 0);
		~Counter();

		void increment();
		int getValue() const;

		// STATIC function - can be called without creating an object
		static int getTotalCounters();
		static void showStatistics();
};

#endif
