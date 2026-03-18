#include <iostream>
using namespace std;

template <typename T>
Pair<T>::Pair(T f, T s) : first(f), second(s) {}

template <typename T>
void Pair<T>::display() 
{
	cout << "First: " << first << ", Second: " << second << endl;
}