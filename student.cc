#include <uC++.h>
#include "student.h"
#include "vending.h"
#include "MPRNG.h"
#include "watcard.h"
#include "office.h"
#include "nameserver.h"

extern MPRNG mprng;

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice 
        &cardOffice, unsigned int id, unsigned int maxPurchases )
: printer(prt), nameServer(nameServer), office(cardOffice), id(id), maxPurchases(maxPurchases)
{
}

void Student::main()
{
    // Random number of drinks to buy
    int buy = mprng(1, maxPurchases);

    // Random flavour
    VendingMachine::Flavours flavor = (VendingMachine::Flavours)mprng(0, 3);

    // Create watcard with $5 balance
    FWATCard card;
    card = office.create(id, 5);

    // Get vending machine from name server
    VendingMachine::VendingMachine *machine;
    machine = nameServer.getMachine(id);
    for (int i=0; i<buy; i++) {
        for (;;) {
            // yield
            yield(mprng(1, 10));

            // attempt to buy, deference future
            VendingMachine::Status status;
            for (;;) {
                try {
                    status = machine->buy(flavor, *card());
                    break;
                } catch (WATCardOffice::Lost e) {
                    card = office.create(id, 5);
                    continue;
                }
            }

            if (status == VendingMachine::BUY) {
                break;
            } else if (status == VendingMachine::STOCK) {
                machine = nameServer.getMachine(id);
            } else if (status == VendingMachine::FUNDS) {
                card = office.transfer(id, machine->cost() + 5, card);
            } else {
                exit(EXIT_FAILURE);
            }
        }
    }
}
