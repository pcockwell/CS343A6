#include <uC++.h>
#include "bank.h"

// Constructor
Bank::Bank (unsigned int numStudents) :
    numStudents(numStudents)
{
    accountBalances = new unsigned int[numStudents];
    funds = new uCondition[numStudents];

    // Initialized to 0
    for ( unsigned int i = 0; i < numStudents; i++ ){
        accountBalances[i] = 0;
    }
}

// Destructor
Bank::~Bank() {
    delete [] accountBalances;
    delete [] funds;
}

// Parent calls to deposit into a student's account
void Bank::deposit( unsigned int id, unsigned int amount )
{
    // money has been deposited, wake up courier (if any)
    accountBalances[id] += amount;
    funds[id].signal();
}

// Courier calls to add money to watcard
void Bank::withdraw( unsigned int id, unsigned int amount )
{
    // wait until enough money has become available
    while( accountBalances[id] < amount ){
        funds[id].wait();
    }
    accountBalances[id] -= amount;
}
