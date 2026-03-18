#ifndef CAR_H
#define CAR_H

class Car
{
	public:
		virtual void display() const = 0;
		virtual ~Car() = default;
};

#endif
