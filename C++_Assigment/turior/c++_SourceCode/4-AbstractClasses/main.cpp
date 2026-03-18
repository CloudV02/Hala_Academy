#include "CreditCardPayment.h"

int main()
{
	PaymentMethod* payment = new CreditCardPayment("1234-5678-9876");
	payment->processPayment(100.0); // Calls the 'processPayment' method of CreditCardPayment
	delete payment;
}
