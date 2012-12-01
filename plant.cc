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
            prt.print( Printer::BottlingPlant, BottlingPlant::Pickup );
            makeProductionRun();
            yield(timeBetweenShipments);
        }
    }

}

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer,
        unsigned int numVendingMachines,
        unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
        unsigned int timeBetweenShipments ) : prt(prt), nameServer(nameServer)
{

    numVendingMachines = numVendingMachines;
    maxShippedPerFlavour = maxShippedPerFlavour;
    maxStockPerFlavour = maxStockPerFlavour;
    timeBetweenShipments = timeBetweenShipments;

    closingDown = false;

    prt.print( Printer::BottlingPlant, BottlingPlant::Start );

    makeProductionRun();

    truck = new Truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour );

}

bool BottlingPlant::getShipment( unsigned int cargo[] )
{
    if ( !closingDown ){
        for ( unsigned int i = 0; i < numFlavours; i++ ){
            cargo[i] += curStock[i];
        }
    }
    return closingDown;
}

BottlingPlant::~BottlingPlant()
{
    closingDown = true;
    _Accept( getShipment );
    delete truck;

    prt.print( Printer::BottlingPlant, BottlingPlant::Finished );
}

void BottlingPlant::makeProductionRun(){
    int numGenerated = 0;
    for ( unsigned int i = 0; i < numFlavours; i++ ){
        curStock[i] = mprng(0, maxShippedPerFlavour);
        numGenerated += curStock[i];
    }
    prt.print( Printer::BottlingPlant, BottlingPlant::Generating, numGenerated );
}
