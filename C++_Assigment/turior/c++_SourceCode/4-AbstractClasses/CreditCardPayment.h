#ifndef CREDITCARDPAYMENT_H
#define CREDITCARDPAYMENT_H

#include "PaymentMethod.h"
#include <string>
using namespace std;

class CreditCardPayment : public PaymentMethod
{
	private:
		string cardNumber;

	public:
		CreditCardPayment(string cardNum); // Constructor
		void processPayment(double amount) override; // Method implementation
};

#endif
