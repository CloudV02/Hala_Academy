#include "Animal.h"
#include "Dog.h"

#include <iostream>
using namespace std;

void interactWithAnimal(Animal* a)
{
    a->speak();

    // Safe downcast to Dog
    if (Dog* d = dynamic_cast<Dog*>(a))
    {
        d->fetch();  // Only if 'a' is really a Dog
    }
    else
    {
        cout << "This animal is not a dog.\n";
    }
}

int main()
{
    Animal* a1 = new Dog;
    Animal* a2 = new Animal;

    interactWithAnimal(a1); // Output: Woof! Dog is fetching a ball.
    interactWithAnimal(a2); // Output: Some animal sound. This animal is not a dog.

    delete a1;
    delete a2;

    return 0;
}
