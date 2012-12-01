#include <uC++.h>
#include "vending.h"
#include "printer.h"
#include "nameserver.h"
#include "watcard.h"

void VendingMachine::main()
{
    while( true ){
        _Accept( ~VendingMachine ){
            break;
        } or _Accept( inventory ){
            _Accept( restocked );
        } or _Accept ( buy ) {

        }
    }

    prt.print( Printer::Vending, machineId, (char)VendingMachine::Finished );
}

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
        unsigned int maxStockPerFlavour ) :
    prt(prt), nameServer(nameServer), machineId(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour)
{

    for (unsigned int i = 0; i < numFlavours; i++){
        inventories[i] = 0;
    }

    prt.print( Printer::Vending, machineId, (char)VendingMachine::Start, sodaCost );

    nameServer.VMregister( this );
}

VendingMachine::Status VendingMachine::buy( Flavours flavour, WATCard &card )
{
    if ( inventories[flavour] == 0 ){
        return STOCK;
    }

    if ( card.getBalance() < sodaCost ){
        return FUNDS;
    }

    card.withdraw( sodaCost );
    inventories[flavour] -= 1;

    prt.print( Printer::Vending, machineId, VendingMachine::SodaBought, flavour, inventories[flavour] );

    return BUY;
}

unsigned int *VendingMachine::inventory()
{
    prt.print( Printer::Vending, machineId, (char)VendingMachine::StartReload );
    return inventories;
}

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

