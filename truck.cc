#include <uC++.h>
#include "truck.h"
#include "MPRNG.h"
#include "vending.h"

void Truck::main()
{
	bool plantClosed = false;
	while( true ){
		yield(mprng(1,10));

		int numBottlesBeforePickup = 0;
		for (unsigned int i = 0; i < this->numFlavours; i++){
			numBottlesBeforePickup += this->cargo[i];
		}

		plantClosed = this->plant.getShipment(cargo);
		if ( plantClosed ){
			break;
		}

		int numBottlesInCargo = 0;
		for (unsigned int i = 0; i < this->numFlavours; i++){
			numBottlesInCargo += this->cargo[i];
		}

		this->prt.print( Printer::Truck, Truck::Pickup, numBottlesInCargo - numBottlesBeforePickup );

		for ( unsigned int i = 0; i < this->numVendingMachines; i++ ){

			if ( numBottlesInCargo == 0 ){
				break;
			}
		
			this->prt.print( Printer::Truck, Truck::BeginDelivery, numBottlesInCargo );

			unsigned int *machineInventory = vendingMachines[i].inventory();
			unsigned int inventoryCount = 0;
			
			for ( unsigned int flavour = 0; flavour < numFlavours; flavour++ ){
				if ( machineInventory[flavour] < this->maxStockPerFlavour && this->cargo[flavour] > 0 ){
	
					unsigned int inventoryRoom = this->maxStockPerFlavour - machineInventory[flavour];
					unsigned int numTransferred = ( this->cargo[flavour] <= inventoryRoom ? this->cargo[flavour] : inventoryRoom );

					machineInventory[flavour] += numTransferred;
					this->cargo[flavour] -= numTransferred;
					numBottlesInCargo -= numTransferred;
				}

				inventoryCount += machineInventory[flavour];
			}

			if ( inventoryCount != this->numFlavours * this->maxStockPerFlavour ){
				this->prt.print( Printer::Truck, Truck::UnsuccessfulFill, inventoryCount - ( this->numFlavours * this->maxStockPerFlavour ) );
			}
		
			this->prt.print( Printer::Truck, Truck::EndDelivery, numBottlesInCargo );

			vendingMachines[i].restocked();
		}

	}
	this->prt.print( Printer::Truck, Truck::Finished );
}

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
      unsigned int numVendingMachines, unsigned int maxStockPerFlavour )
		: prt(prt), nameServer(nameServer), plant(plant)
{
	this->numVendingMachines = numVendingMachines;
	this->maxStockPerFlavour = maxStockPerFlavour;
	
	this->prt.print( Printer::Truck, Truck::Start );

	for (unsigned int i = 0; i < this->numFlavours; i++){
		cargo[i] = 0;
	}

	this->vendingMachines = nameServer.getMachineList();
}

