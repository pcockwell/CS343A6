#include <uC++.h>
#include "plant.h"
#include "nameserver.h"
#include "MPRNG.h"
#include "printer.h"
#include "truck.h"

extern MPRNG mprng;

// Constructor
BottlingPlant::BottlingPlant( Printer &prt,
                                NameServer &nameServer,
                                unsigned int numVendingMachines,
                                unsigned int maxShippedPerFlavour,
                                unsigned int maxStockPerFlavour,
                                unsigned int timeBetweenShipments ) :
    prt(prt),
    nameServer(nameServer),
    numVendingMachines(numVendingMachines),
    maxShippedPerFlavour(maxShippedPerFlavour),
    maxStockPerFlavour(maxStockPerFlavour),
    timeBetweenShipments(timeBetweenShipments)
{
    closingDown = false;

    prt.print( Printer::BottlingPlant, (char)BottlingPlant::Start );

    truck = new Truck( prt,
                        nameServer,
                        *this,
                        numVendingMachines,
                        maxStockPerFlavour );
}

// Destructor
BottlingPlant::~BottlingPlant()
{
    closingDown = true;
    _Accept( getShipment ); // allow truck to terminate
    delete truck;

    prt.print( Printer::BottlingPlant, (char)BottlingPlant::Finished );
}

void BottlingPlant::main()
{
    // produce some bottles initially
    makeProductionRun();
    while( true ) {
        _Accept( ~BottlingPlant ) { // allow plant to terminate
            break;
        } or _Accept( getShipment ) { // truck picks up shipment from a production run
            prt.print( Printer::BottlingPlant, BottlingPlant::Pickup );
            yield(timeBetweenShipments);
            makeProductionRun();
        }
    }
}

bool BottlingPlant::getShipment( unsigned int cargo[] )
{
    if ( !closingDown ){
        for ( unsigned int i = 0; i < numFlavours; i++ ){
            cargo[i] += curStock[i]; // fill the cargo
        }
    }

    return closingDown;
}

// randomly produce some number of soda of each flavour
void BottlingPlant::makeProductionRun() {
    int numGenerated = 0;
    for ( unsigned int i = 0; i < numFlavours; i++ ) {
        curStock[i] = mprng(0, maxShippedPerFlavour);
        numGenerated += curStock[i];
    }
    prt.print( Printer::BottlingPlant, (char)BottlingPlant::Generating, numGenerated );
}
