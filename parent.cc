#include <uC++.h>
#include "parent.h"

Parent::Parent(Printer& prt, Bank& bank, unsigned int numStudents,
      unsigned int parentalDelay) : prt(prt), bank(bank)
{
	this->numStudents = numStudents;
	this->parentalDelay = parentalDelay;

	this->prt.print(Printer::Parent, Parent::Start);
}

void Parent::main()
{
	while(true){
		_Accept( ~Parent ){
			break;
		} else {
			yield(this->parentalDelay);
			
			unsigned int studentId = mprng(1, numStudents);
			unsigned int amount = mprng(1, 3);

			this->prt.print(Printer::Parent, Parent::Deposit, studentId, amount);
			bank.deposit(studentId, amount);
		}
	}

	this->prt.print(Printer::Parent, Parent::Finished);
}
