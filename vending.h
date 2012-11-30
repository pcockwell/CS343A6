#ifndef __VENDING_H__
#define __VENDING_H__

_Monitor Printer;
_Task NameServer;
class WATCard;

_Task VendingMachine {
   private:
      void main();
   public:
      enum Flavours { FA, FB, FC, FD }; 			// flavours of soda (YOU DEFINE)
      enum Status { BUY, STOCK, FUNDS };		// purchase status: successful buy, out of stock, insufficient funds
      VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
            unsigned int maxStockPerFlavour );
      Status buy( Flavours flavour, WATCard &card );
      unsigned int *inventory();
      void restocked();
      _Nomutex unsigned int cost();
      _Nomutex unsigned int getId();
};

#endif
