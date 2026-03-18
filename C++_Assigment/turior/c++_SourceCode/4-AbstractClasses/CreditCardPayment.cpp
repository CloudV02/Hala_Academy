#include "CreditCardPayment.h"
#include <iostream>

CreditCardPayment::CreditCardPayment(string cardNum) : cardNumber(cardNum) {}

void CreditCardPayment::processPayment(double amount)
{
	cout << "Processing Credit Card payment of " << amount << " for card " << cardNumber << endl;
}
