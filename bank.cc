#include <uC++.h>
#include "bank.h"

Bank::Bank (unsigned int numStudents)
{
    this->numStudents = numStudents;

    this->accountBalances = new unsigned int[this->numStudents];
    this->funds = new uCondition[this->numStudents];

    for ( unsigned int i = 0; i < this->numStudents; i++ ){
        this->accountBalances[i] = 0;
    }
}

Bank::~Bank(){
    delete accountBalances;
    delete funds;
}

void Bank::deposit( unsigned int id, unsigned int amount )
{
    this->accountBalances[id] += amount;
    this->funds[id].signal();
}

void Bank::withdraw( unsigned int id, unsigned int amount )
{
    while( this->accountBalances[id] < amount ){
        this->funds[id].wait();
    }
    this->accountBalances[id] -= amount;
}
