#include <uC++.h>
#include <uFuture.h>
#include <cassert>
#include "watcard.h"


// Constructor
WATCard::WATCard() : balance(0)
{
}

// courier calls after withdrawing from bank
void WATCard::deposit( unsigned int amount )
{
    balance += amount;
}

// venidng machine calls to debit watcard for soda purchase
void WATCard::withdraw( unsigned int amount )
{
    assert(amount <= balance);
    balance -= amount;
}

unsigned int WATCard::getBalance()
{
    return balance;
}

