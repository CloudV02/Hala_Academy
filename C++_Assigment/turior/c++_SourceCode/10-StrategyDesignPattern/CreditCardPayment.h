#ifndef CREDITCARDPAYMENT_H
#define CREDITCARDPAYMENT_H

#include "PaymentStrategy.h"

class CreditCardPayment : public PaymentStrategy
{
public:
	void pay(int amount) override;
};

#endif
