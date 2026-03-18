#ifndef PAYPALPAYMENT_H
#define PAYPALPAYMENT_H

#include "PaymentStrategy.h"

class PaypalPayment : public PaymentStrategy
{
public:
	void pay(int amount) override;
};

#endif
