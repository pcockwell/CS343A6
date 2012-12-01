#include <uC++.h>
#include "truck.h"
#include "MPRNG.h"
#include "vending.h"
#include "printer.h"
#include "nameserver.h"
#include "plant.h"

extern MPRNG mprng;

void Truck::main()
{
    bool plantClosed = false;
    while( true ){
        yield(mprng(1,10));

        int numBottlesBeforePickup = 0;
        for (unsigned int i = 0; i < numFlavours; i++){
            numBottlesBeforePickup += cargo[i];
        }

        plantClosed = plant.getShipment(cargo);
        if ( plantClosed ){
            break;
        }

        int numBottlesInCargo = 0;
        for (unsigned int i = 0; i < numFlavours; i++){
            numBottlesInCargo += cargo[i];
        }

        prt.print( Printer::Truck, Truck::Pickup, numBottlesInCargo - numBottlesBeforePickup );

        for ( unsigned int i = 0; i < numVendingMachines; i++ ){

            if ( numBottlesInCargo == 0 ){
                break;
            }

            prt.print( Printer::Truck, Truck::BeginDelivery, numBottlesInCargo );

            unsigned int *machineInventory = vendingMachines[i]->inventory();
            unsigned int inventoryCount = 0;

            for ( unsigned int flavour = 0; flavour < numFlavours; flavour++ ){
                if ( machineInventory[flavour] < maxStockPerFlavour && cargo[flavour] > 0 ){

                    unsigned int inventoryRoom = maxStockPerFlavour - machineInventory[flavour];
                    unsigned int numTransferred = ( cargo[flavour] <= inventoryRoom ? cargo[flavour] : inventoryRoom );

                    machineInventory[flavour] += numTransferred;
                    cargo[flavour] -= numTransferred;
                    numBottlesInCargo -= numTransferred;
                }

                inventoryCount += machineInventory[flavour];
            }

            if ( inventoryCount != numFlavours * maxStockPerFlavour ){
                prt.print( Printer::Truck, (char)Truck::UnsuccessfulFill, inventoryCount - ( numFlavours * maxStockPerFlavour ) );
            }

            prt.print( Printer::Truck, Truck::EndDelivery, numBottlesInCargo );

            vendingMachines[i]->restocked();
        }

    }
    prt.print( Printer::Truck, Truck::Finished );
}

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
        unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
    prt(prt), nameServer(nameServer), plant(plant), numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour)
{
    for (unsigned int i = 0; i < numFlavours; i++){
        cargo[i] = 0;
    }

    vendingMachines = nameServer.getMachineList();

    prt.print( Printer::Truck, Truck::Start );
}