#include <uC++.h>
#include "parent.h"
#include "printer.h"
#include "MPRNG.h"
#include "bank.h"

extern MPRNG mprng;

Parent::Parent(Printer& prt, Bank& bank, unsigned int numStudents, unsigned int parentalDelay) :
    prt(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay)
{
    prt.print(Printer::Parent, (char)Parent::Start);
}

Parent::~Parent()
{
    prt.print(Printer::Parent, (char)Parent::Finished);
}

void Parent::main()
{
    while(true){
        _Accept( ~Parent ){
            break;
        } else {
            yield(parentalDelay);

            unsigned int studentId = mprng(0, numStudents - 1);
            unsigned int amount = mprng(1, 3);

            prt.print(Printer::Parent, (char)Parent::Deposit, studentId, amount);
            bank.deposit(studentId, amount);
        }
    }

}
