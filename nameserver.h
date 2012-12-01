#ifndef __NAME_SERVER__
#define __NAME_SERVER__

_Monitor Printer;
_Monitor Bank;
_Task VendingMachine;

_Task NameServer {
    private:
        Printer &prt;

        unsigned int numStudents;
        unsigned int numVendingMachines;
        //unsigned int nextRegisterId;
        unsigned int registeredMachines;

        VendingMachine** vendingMachines;
        unsigned int* assignedMachines;

        enum States { Start = 'S', Register = 'R', NewMachine = 'N', Finished = 'F' };

        void main();
    public:
        NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
        ~NameServer();

        void VMregister( VendingMachine *vendingmachine );
        VendingMachine *getMachine( unsigned int id );
        VendingMachine **getMachineList();
};

#endif
