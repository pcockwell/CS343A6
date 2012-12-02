#include <uC++.h>
#include "student.h"
#include "vending.h"
#include "MPRNG.h"
#include "watcard.h"
#include "office.h"
#include "nameserver.h"
#include "printer.h"

extern MPRNG mprng;

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice 
        &cardOffice, unsigned int id, unsigned int maxPurchases )
: prt(prt), nameServer(nameServer), office(cardOffice), id(id), maxPurchases(maxPurchases)
{
}

void Student::main()
{
    // Random number of drinks to buy
    int buy = mprng(1, maxPurchases);

    // Random flavour
    VendingMachine::Flavours flavour = (VendingMachine::Flavours)mprng(0, 3);

    prt.print( Printer::Student, id, (char)Student::Start, flavour, buy );

    // Create watcard with $5 balance
    FWATCard card;
    card = office.create(id, 5);

    // Get vending machine from name server
    VendingMachine::VendingMachine *machine;
    machine = nameServer.getMachine(id);

    prt.print( Printer::Student, id, (char)Student::VendingMachine, machine->getId() );

    for (int i=0; i<buy; i++) {
        for (;;) {
            // yield
            yield(mprng(1, 10));

            // attempt to buy, deference future
            VendingMachine::Status status;
            for (;;) {
                try {
                    status = machine->buy(flavour, *card());
                    break;
                } catch (WATCardOffice::Lost e) {
                    prt.print( Printer::Student, id, (char)Student::WATCardLost );
                    card = office.create(id, 5);
                    continue;
                }
            }

            if (status == VendingMachine::BUY) {
                prt.print( Printer::Student, id, (char)Student::SodaBought, (*card()).getBalance() );
                break;
            } else if (status == VendingMachine::STOCK) {
                machine = nameServer.getMachine(id);
                prt.print( Printer::Student, id, (char)Student::VendingMachine, machine->getId() );
            } else if (status == VendingMachine::FUNDS) {
                card = office.transfer(id, machine->cost() + 5, card);
            } else {
                exit(EXIT_FAILURE);
            }
        }
    }
    delete card;
    prt.print( Printer::Student, id, (char)Student::Finished );
}
