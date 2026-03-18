#ifndef OBSERVER_H
#define OBSERVER_H

class Observer
{
public:
	virtual void update(float temperature) = 0;
	virtual ~Observer() = default;
};

#endif
