#include <uC++.h>
#include "nameserver.h"
#include "vending.h"

void NameServer::main()
{
	while( this->registeredMachines < this->numVendingMachines ){
		_Accept( VMregister ){
			this->registeredMachines++;
		}
	} 

	_Accept( getMachineList );

	while ( true ){
		_Accept( ~NameServer ){
			break;
		} or _Accept( getMachine ){

		}
	}
}

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned 
      int numStudents ) :prt(prt)
{
	this->numStudents = numStudents;
	this->numVendingMachines = numVendingMachines;
	this->registeredMachines = 0;

	this->vendingMachines = new VendingMachine*[numVendingMachines];
	this->assignedMachines = new unsigned int[numStudents];

	for ( unsigned int i = 0; i < this->numStudents; i++ ){
		this->assignedMachines[i] = i % this->numVendingMachines;
	}
	this->prt.print( Printer::NameServer, NameServer::Start );
}

NameServer::~NameServer(){
	this->prt.print( Printer::NameServer, NameServer::Finished );
	delete vendingMachines;
	delete assignedMachines;
}

void NameServer::VMregister( VendingMachine *vendingmachine )
{
	this->prt.print( Printer::NameServer, NameServer::Register, vendingmachine->getId() );
	this->vendingMachines[this->registeredMachines] = vendingmachine;
}

VendingMachine* NameServer::getMachine( unsigned int id )
{
	unsigned int machineId = this->assignedMachines[id];

	this->prt.print( Printer::NameServer, NameServer::NewMachine, id, machineId );

	VendingMachine* returnedMachine = this->VendingMachines[machineId];
	this->assignedMachines[id] = (this->assignedMachines[id] + 1) % numVendingMachines;
	return returnedMachine;
}

VendingMachine** NameServer::getMachineList()
{
	return this->vendingMachines;
}