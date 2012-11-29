#ifndef __PLANT_H__
#define __PLANT_H__

_Monitor Printer;
_Task NameServer;

_Task BottlingPlant {
   private:
      void main();
   public:
      BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
            unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
            unsigned int timeBetweenShipments );
      bool getShipment( unsigned int cargo[] );
};

#endif
