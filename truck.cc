#include <uC++.h>
#include "truck.h"
#include "MPRNG.h"
#include "vending.h"
#include "printer.h"
#include "nameserver.h"
#include "plant.h"

extern MPRNG mprng;

// Constructor
Truck::Truck( Printer &prt,
                NameServer &nameServer,
                BottlingPlant &plant,
                unsigned int numVendingMachines,
                unsigned int maxStockPerFlavour ) :
    prt(prt),
    nameServer(nameServer),
    plant(plant),
    numVendingMachines(numVendingMachines),
    maxStockPerFlavour(maxStockPerFlavour)
{
    for (unsigned int i = 0; i < numFlavours; i++){
        cargo[i] = 0;
    }

    // get list of vending machines
    vendingMachines = nameServer.getMachineList();

    prt.print( Printer::Truck, Truck::Start );
}

void Truck::main()
{
    bool plantClosed = false;
    while ( true ) {
        yield(mprng(1,10));

        // number of leftover bottles
        int numBottlesBeforePickup = 0;
        for (unsigned int i = 0; i < numFlavours; i++){
            numBottlesBeforePickup += cargo[i];
        }

        // terminating
        plantClosed = plant.getShipment(cargo);
        if ( plantClosed ){
            break;
        }

        // number of bottles now
        int numBottlesInCargo = 0;
        for (unsigned int i = 0; i < numFlavours; i++){
            numBottlesInCargo += cargo[i];
        }

        prt.print( Printer::Truck,
                    (char)Truck::Pickup,
                    numBottlesInCargo - numBottlesBeforePickup );

        for ( unsigned int i = 0; i < numVendingMachines; i++ ){
            // stop of no more soda left on the truck
            if ( numBottlesInCargo == 0 ) {
                break;
            }

            prt.print( Printer::Truck,
                        (char)Truck::BeginDelivery,
                        vendingMachines[i]->getId(),
                        numBottlesInCargo );

            unsigned int *machineInventory = vendingMachines[i]->inventory();
            unsigned int inventoryCount = 0;

            // refill each flavour
            for ( unsigned int flavour = 0; flavour < numFlavours; flavour++ ){
                // have space in vending machine and have stock
                if ( machineInventory[flavour] < maxStockPerFlavour && cargo[flavour] > 0 ){

                    unsigned int inventoryRoom = maxStockPerFlavour - machineInventory[flavour];
                    unsigned int numTransferred = ( cargo[flavour] <= inventoryRoom ? cargo[flavour]
                                                                                    : inventoryRoom );

                    machineInventory[flavour] += numTransferred;
                    cargo[flavour] -= numTransferred;
                    numBottlesInCargo -= numTransferred;
                }

                inventoryCount += machineInventory[flavour];
            }

            // couldn't fully restock the vending machine
            if ( inventoryCount != numFlavours * maxStockPerFlavour ){
                prt.print( Printer::Truck,
                            (char)Truck::UnsuccessfulFill,
                            vendingMachines[i]->getId(),
                            numFlavours * maxStockPerFlavour - inventoryCount );
            }

            prt.print( Printer::Truck,
                        (char)Truck::EndDelivery,
                        vendingMachines[i]->getId(),
                        numBottlesInCargo );

            // restock complete
            vendingMachines[i]->restocked();
        }
    }
    prt.print( Printer::Truck, (char)Truck::Finished );
}

