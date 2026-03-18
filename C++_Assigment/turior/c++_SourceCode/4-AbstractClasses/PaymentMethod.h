#ifndef PAYMENTMETHOD_H
#define PAYMENTMETHOD_H

class PaymentMethod
{
	public:
		virtual void processPayment(double amount) = 0; // Pure virtual function
		virtual ~PaymentMethod() {}
};

#endif
