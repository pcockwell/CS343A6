#include <uC++.h>
#include "nameserver.h"
#include "vending.h"
#include "printer.h"

// Constructor
NameServer::NameServer( Printer &prt,
                        unsigned int numVendingMachines,
                        unsigned int numStudents ) :
    prt(prt),
    numStudents(numStudents),
    numVendingMachines(numVendingMachines),
    registeredMachines(0)
{
    vendingMachines = new VendingMachine*[numVendingMachines];
    assignedMachines = new unsigned int[numStudents];

    // assign students to a vending machine using round robin
    for ( unsigned int i = 0; i < numStudents; i++ ){
        assignedMachines[i] = i % numVendingMachines;
    }
    prt.print( Printer::NameServer, (char)NameServer::Start );
}

// Destructor
NameServer::~NameServer()
{
    prt.print( Printer::NameServer, (char)NameServer::Finished );
    delete vendingMachines;
    delete assignedMachines;
}

void NameServer::main()
{
    // Make sure all machines are registered before allowing students to look up 
    // machines
    while( registeredMachines < numVendingMachines ) {
        _Accept( VMregister ) {
            registeredMachines++;
        }
    } 

    // truck call once to obtain locations of vending machines so it may restock 
    // them
    _Accept( getMachineList );

    while ( true ) {
        _Accept( ~NameServer ) { // allow name server to terminate
            break;
        } or _Accept( getMachine ) { // student look up a vending machine

        }
    }
}

// VendingMachine call to register themselves
void NameServer::VMregister( VendingMachine *vendingmachine )
{
    prt.print( Printer::NameServer,
            (char)NameServer::Register,
            vendingmachine->getId() );
    vendingMachines[registeredMachines] = vendingmachine;
}

// student call to look up a vending machine
VendingMachine* NameServer::getMachine( unsigned int id )
{
    unsigned int machineId = assignedMachines[id];

    prt.print( Printer::NameServer, (char)NameServer::NewMachine, id, machineId );

    VendingMachine* returnedMachine = vendingMachines[machineId];
    assignedMachines[id] = (assignedMachines[id] + 1) % numVendingMachines;
    return returnedMachine;
}

// truck call to get a list of vending machines that it should subsequently 
// restock
VendingMachine** NameServer::getMachineList()
{
    return vendingMachines;
}
