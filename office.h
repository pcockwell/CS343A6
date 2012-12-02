#ifndef __OFFICE_H__
#define __OFFICE_H__

#include <uC++.h>
#include <queue>
#include "watcard.h"

_Monitor Printer;
_Monitor Bank;

_Task WATCardOffice {
    private:
        struct Args {
            unsigned int   sid;
            unsigned int   amount;
            WATCard        *card;
            bool            termination;
        };

        struct Job {				// marshalled arguments and return future
            Args     args;				// call arguments (YOU DEFINE "Args")
            FWATCard result;			// return future
            Job( Args args ) : args( args ) {}
        };

        _Task Courier { // communicates with bank
            private:
                Printer        &prt;
                WATCardOffice  &office;
                Bank            &bank;

                unsigned int id;

                enum States { Start = 'S', StartTransfer = 't', CompleteTransfer = 'T', Finished = 'F' };

                void main();
            public:
                Courier(Printer &prt, WATCardOffice& cardOffice, Bank& bank, unsigned int id);
                ~Courier();
        };

        enum States { Start = 'S', CourierComplete = 'W', CreationComplete = 'C', TransferComplete = 'T', Finished = 'F' };

        void main();

        Printer&       prt;
        Bank&          bank;
        unsigned int   numCouriers;

        uCondition        jobReady;
        std::queue<Job *> jobs;

        std::vector<Courier *> couriers;

    public:
        _Event Lost {};
        WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
        FWATCard create( unsigned int sid, unsigned int amount );
        FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
        Job *requestWork();
        ~WATCardOffice ();
};

#endif
