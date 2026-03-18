#ifndef PAIR_H
#define PAIR_H

template <typename T>
class Pair
{
	private:
		T first;
		T second;

	public:
		Pair(T f, T s);
		void display();
};

#include "Pair.tpp"

#endif
