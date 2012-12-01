#ifndef __TRUCK_H__
#define __TRUCK_H__

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;
_Task VendingMachine;;

_Task Truck {
    private:
        Printer &prt;
        NameServer &nameServer;
        BottlingPlant &plant;

        VendingMachine** vendingMachines;

        const static unsigned int numFlavours = 4;

        unsigned int cargo[numFlavours];
        unsigned int numVendingMachines;
        unsigned int maxStockPerFlavour;

        enum States { Start = 'S', Pickup = 'P', BeginDelivery = 'd', UnsuccessfulFill = 'U', EndDelivery = 'D', Finished = 'F' };

        void main();
    public:
        Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
                unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
