#ifndef PAYMENTSTRATEGY_H
#define PAYMENTSTRATEGY_H

class PaymentStrategy
{
public:
	virtual void pay(int amount) = 0;
	virtual ~PaymentStrategy() = default;
};

#endif
