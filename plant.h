#ifndef __PLANT_H__
#define __PLANT_H__

_Monitor Printer;
_Task NameServer;
_Task Truck;

_Task BottlingPlant {
    private:
        Printer     &prt;
        NameServer  &nameServer;
        Truck       *truck;

        unsigned int numVendingMachines;
        unsigned int maxShippedPerFlavour;
        unsigned int maxStockPerFlavour;
        unsigned int timeBetweenShipments;

        bool closingDown;   // flag to indicate plant is terminating

        const static unsigned int numFlavours = 4;

        // current stock level as produced by a production run
        int curStock[numFlavours];

        enum States { Start = 'S', Generating = 'G', Pickup = 'P', Finished = 'F' };

        void main();
        void makeProductionRun();
    public:
        BottlingPlant( Printer &prt,
                        NameServer &nameServer,
                        unsigned int numVendingMachines,
                        unsigned int maxShippedPerFlavour,
                        unsigned int maxStockPerFlavour,
                        unsigned int timeBetweenShipments );
        ~BottlingPlant();
        bool getShipment( unsigned int cargo[] );
};

#endif
