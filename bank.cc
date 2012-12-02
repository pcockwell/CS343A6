#include <uC++.h>
#include "bank.h"

Bank::Bank (unsigned int numStudents) :
    numStudents(numStudents)
{
    accountBalances = new unsigned int[numStudents];
    funds = new uCondition[numStudents];

    for ( unsigned int i = 0; i < numStudents; i++ ){
        accountBalances[i] = 0;
    }
}

Bank::~Bank(){
    delete [] accountBalances;
    delete [] funds;
}

void Bank::deposit( unsigned int id, unsigned int amount )
{
    accountBalances[id] += amount;
    funds[id].signal();
}

void Bank::withdraw( unsigned int id, unsigned int amount )
{
    while( accountBalances[id] < amount ){
        funds[id].wait();
    }
    accountBalances[id] -= amount;
}
