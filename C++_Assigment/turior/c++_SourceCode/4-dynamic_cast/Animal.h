#ifndef ANIMAL_H
#define ANIMAL_H

class Animal
{
public:
	Animal();
    virtual void speak();
    virtual ~Animal() = default;
};

#endif
