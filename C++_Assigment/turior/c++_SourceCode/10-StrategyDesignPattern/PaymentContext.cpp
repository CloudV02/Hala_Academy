#include "PaymentContext.h"
#include "PaymentStrategy.h"

PaymentContext::PaymentContext(shared_ptr<PaymentStrategy> initialStrategy) : strategy(initialStrategy) {}

void PaymentContext::setPaymentStrategy(shared_ptr<PaymentStrategy> newStrategy)
{
	strategy = newStrategy;
}

void PaymentContext::executePayment(int amount)
{
	strategy->pay(amount);
}
