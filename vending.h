#ifndef __VENDING_H__
#define __VENDING_H__

_Monitor Printer;
_Task NameServer;
class WATCard;

_Task VendingMachine {
    private:
        Printer     &prt;
        NameServer  &nameServer;

        unsigned int machineId;
        unsigned int sodaCost;
        unsigned int maxStockPerFlavour;

        const static unsigned int numFlavours = 4;

        // inventory count for each flavour
        unsigned int inventories[numFlavours];

        enum States { Start = 'S', StartReload = 'r', EndReload = 'R', SodaBought = 'B', Finished = 'F' };

        void main();

    public:
        enum Flavours { FA = 0, FB, FC, FD }; 
        // purchase status: successful buy, out of stock, insufficient funds
        enum Status { BUY, STOCK, FUNDS };

        VendingMachine( Printer &prt,
                        NameServer &nameServer,
                        unsigned int id,
                        unsigned int sodaCost,
                        unsigned int maxStockPerFlavour );

        Status          buy ( Flavours flavour, WATCard &card );
        unsigned int    *inventory ();
        void            restocked();

        _Nomutex unsigned int cost();
        _Nomutex unsigned int getId();
};

#endif
