#include <uC++.h>
#include "plant.h"
#include "nameserver.h"
#include "MPRNG.h"
#include "printer.h"
#include "truck.h"

extern MPRNG mprng;

void BottlingPlant::main()
{

	while( true ){
		_Accept( ~BottlingPlant ){
			break;
		} or _Accept( getShipment ){
			this->prt.print( Printer::BottlingPlant, BottlingPlant::Pickup );
			this->makeProductionRun();
			yield(this->timeBetweenShipments);
		}
	}

}

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer,
      unsigned int numVendingMachines,
      unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
      unsigned int timeBetweenShipments ) : prt(prt), nameServer(nameServer)
{

	this->numVendingMachines = numVendingMachines;
	this->maxShippedPerFlavour = maxShippedPerFlavour;
	this->maxStockPerFlavour = maxStockPerFlavour;
	this->timeBetweenShipments = timeBetweenShipments;

	this->closingDown = false;

	this->prt.print( Printer::BottlingPlant, BottlingPlant::Start );

	this->makeProductionRun();

	truck = new Truck( prt, nameServer, this, numVendingMachines, maxStockPerFlavour );

}

bool BottlingPlant::getShipment( unsigned int cargo[] )
{
	if ( !this->closingDown ){
		for ( unsigned int i = 0; i < this->numFlavours; i++ ){
			cargo[i] = curStock[i];
		}
	}
	return this->closingDown;
}

bool BottlingPlant::~BottlingPlant()
{
	this->closingDown = true;
	_Accept( getShipment );

	this->prt.print( Printer::BottlingPlant, BottlingPlant::Finished );
}

void BottlingPlant::makeProductionRun(){
	int numGenerated = 0;
	for ( unsigned int i = 0; i < this->numFlavours; i++ ){
		curStock[i] = mprng(0, this->maxShippedPerFlavour);
		numGenerated += curStock[i];
	}
	this->prt.print( Printer::BottlingPlant, BottlingPlant::Generating, numGenerated );
}