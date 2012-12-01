#include <uC++.h>
#include "vending.h"

void VendingMachine::main()
{
	while( true ){
		_Accept( ~VendingMachine ){
			break;
		} or _Accept( inventory ){
			_Accept( restocked );
		}
	}
	
	this->prt.print( Printer::Vending, this->machineId, VendingMachine::Finished );
}

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
      unsigned int maxStockPerFlavour ) : prt(prt), nameServer(nameServer)
{
	this->machineId = id;
	this->sodaCost = sodaCost;
	this->maxStockPerFlavour = maxStockPerFlavour;

	for (unsigned int i = 0; i < this->numFlavours; i++){
		this->inventory[i] = 0;
	}
	
	this->prt.print( Printer::Vending, this->machineId, VendingMachine::Start );

	nameServer.VMregister( this );
}

VendingMachine::Status VendingMachine::buy( Flavours flavour, WATCard &card )
{
	if ( inventory[flavour] == 0 ){
		return STOCK;
	}

	if ( card->getBalance() < this->sodaCost ){
		return FUNDS;
	}

	card->withdraw( this->sodaCost );
	inventory[flavour] -= 1;

	this->prt.print( Printer::Vending, this->machineId, VendingMachine::SodaBought, flavour, inventory[flavour] );

	return BUY;
}

unsigned int *VendingMachine::inventory()
{
	this->prt.print( Printer::Vending, this->machineId, VendingMachine::StartReload );
	return this->inventory;
}

void VendingMachine::restocked()
{
	this->prt.print( Printer::Vending, this->machineId, VendingMachine::EndReload );
}

_Nomutex unsigned int VendingMachine::cost()
{
	return this->sodaCost;
}

_Nomutex unsigned int VendingMachine::getId()
{
	return this->machineId;
}

