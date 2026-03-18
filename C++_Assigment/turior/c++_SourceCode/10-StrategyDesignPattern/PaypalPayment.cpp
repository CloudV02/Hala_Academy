#include "PaypalPayment.h"
#include <iostream>
using namespace std;

void PaypalPayment::pay(int amount)
{
	cout << "Paid " << amount << " using PayPal." << endl;
}
