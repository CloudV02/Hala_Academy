#ifndef PAYMENTCONTEXT_H
#define PAYMENTCONTEXT_H

#include "PaymentStrategy.h"
#include <memory>
using namespace std;

class PaymentContext
{
	private:
		shared_ptr<PaymentStrategy> strategy;
	public:
		PaymentContext(shared_ptr<PaymentStrategy> initialStrategy);
		void setPaymentStrategy(shared_ptr<PaymentStrategy> newStrategy);
		void executePayment(int amount);
};

#endif
