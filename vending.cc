#include <uC++.h>
#include "vending.h"
#include "printer.h"
#include "nameserver.h"
#include "watcard.h"

// Constructor
VendingMachine::VendingMachine( Printer &prt,
                                NameServer &nameServer,
                                unsigned int id,
                                unsigned int sodaCost,
                                unsigned int maxStockPerFlavour ) :
    prt(prt), nameServer(nameServer), machineId(id),
    sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour)
{
    for (unsigned int i = 0; i < numFlavours; i++){
        inventories[i] = 0;
    }
    prt.print( Printer::Vending,
                machineId,
                (char)VendingMachine::Start,
                sodaCost );
}

void VendingMachine::main()
{
    nameServer.VMregister( this );
    while( true ) {
        _Accept( ~VendingMachine ) {
            break;  // allow VendingMachine to terminate
        } or _Accept( inventory ) {
            _Accept( restocked );   // purchase not allowed while restocking
        } or _Accept ( buy ) {

        }
    }

    prt.print( Printer::Vending, machineId, (char)VendingMachine::Finished );
}

// Attempt to purchase soda of this flavour using card
VendingMachine::Status VendingMachine::buy( Flavours flavour, WATCard &card )
{
    // not enough stock
    if ( inventories[flavour] == 0 ){
        return STOCK;
    }

    // not enough balance
    if ( card.getBalance() < sodaCost ){
        return FUNDS;
    }

    // debit the watcard
    card.withdraw( sodaCost );
    inventories[flavour] -= 1;

    prt.print( Printer::Vending,
                machineId,
                VendingMachine::SodaBought,
                flavour,
                inventories[flavour] );

    // transaction succeeds
    return BUY;
}

// start reloading by truck
unsigned int *VendingMachine::inventory()
{
    prt.print( Printer::Vending, machineId, (char)VendingMachine::StartReload );
    return inventories;
}

// truck has finished restocking
void VendingMachine::restocked()
{
    prt.print( Printer::Vending, machineId, (char)VendingMachine::EndReload );
}

_Nomutex unsigned int VendingMachine::cost()
{
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId()
{
    return machineId;
}

