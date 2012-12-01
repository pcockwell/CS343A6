#include <uC++.h>
#include "nameserver.h"
#include "vending.h"
#include "printer.h"

void NameServer::main()
{
    while( registeredMachines < numVendingMachines ){
        _Accept( VMregister ){
            registeredMachines++;
        }
    } 

    _Accept( getMachineList );

    while ( true ) {
        _Accept( ~NameServer ){
            break;
        } or _Accept( getMachine ){

        }
    }
}

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines,
        unsigned int numStudents ) : prt(prt)
{
    numStudents = numStudents;
    numVendingMachines = numVendingMachines;
    registeredMachines = 0;

    vendingMachines = new VendingMachine*[numVendingMachines];
    assignedMachines = new unsigned int[numStudents];

    for ( unsigned int i = 0; i < numStudents; i++ ){
        assignedMachines[i] = i % numVendingMachines;
    }
    prt.print( Printer::NameServer, NameServer::Start );
}

NameServer::~NameServer()
{
    prt.print( Printer::NameServer, NameServer::Finished );
    delete vendingMachines;
    delete assignedMachines;
}

void NameServer::VMregister( VendingMachine *vendingmachine )
{
    prt.print( Printer::NameServer, (char)NameServer::Register, vendingmachine->getId() );
    vendingMachines[registeredMachines] = vendingmachine;
}

VendingMachine* NameServer::getMachine( unsigned int id )
{
    unsigned int machineId = assignedMachines[id];

    prt.print( Printer::NameServer, (char)NameServer::NewMachine, id, machineId );

    VendingMachine* returnedMachine = vendingMachines[machineId];
    assignedMachines[id] = (assignedMachines[id] + 1) % numVendingMachines;
    return returnedMachine;
}

VendingMachine** NameServer::getMachineList()
{
    return vendingMachines;
}
