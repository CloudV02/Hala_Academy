#include <iostream>
#include <unordered_map>
using namespace std;

int main()
{
	unordered_map<string, int> populationMap;

	// Adding key-value pairs to the unordered_map
	populationMap["USA"] = 331002651;
	populationMap["India"] = 1380004385;
	populationMap["China"] = 1439323776;

	// Accessing elements by key
	cout << "Population of India: " << populationMap["India"] << endl;

	populationMap["India"] = 1380004387;

	cout << "Population of India: " << populationMap["India"] << endl;

	for(const auto& pair : populationMap)
	{
		cout << "The population of : " << pair.first << " is " << pair.second << endl;
	}

	return 0;
}
