#ifndef __NAME_SERVER__
#define __NAME_SERVER__

_Monitor Printer;
_Monitor Bank;
_Task VendingMachine;

_Task NameServer {
   private:

      enum States { Start = 'S', Register = 'R', NewMachine = 'N', Finished = 'F' };

      void main();
   public:
      NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
      void VMregister( VendingMachine *vendingmachine );
      VendingMachine *getMachine( unsigned int id );
      VendingMachine **getMachineList();
};

#endif
