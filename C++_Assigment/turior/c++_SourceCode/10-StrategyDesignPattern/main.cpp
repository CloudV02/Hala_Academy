#include "PaymentContext.h"
#include "CreditCardPayment.h"
#include "PaypalPayment.h"
#include <memory>
using namespace std;

int main()
{
	PaymentContext paymentContext(make_shared<CreditCardPayment>());
	paymentContext.executePayment(100);

	paymentContext.setPaymentStrategy(make_shared<PaypalPayment>());
	paymentContext.executePayment(200);

	return 0;
}
