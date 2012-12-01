#include <uC++.h>
#include <uFuture.h>
#include <cassert>
#include "watcard.h"


WATCard::WATCard() : balance(0)
{
}

void WATCard::deposit( unsigned int amount )
{
    balance += amount;
}

void WATCard::withdraw( unsigned int amount )
{
    assert(amount <= balance);
    balance -= amount;
}

unsigned int WATCard::getBalance()
{
    return balance;
}

