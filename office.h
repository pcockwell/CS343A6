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
            unsigned int   sid;     // student id
            unsigned int   amount;  // amount to deposit/transfer
            WATCard        *card;   // which watcard?
            bool        termination;// special flag to terminate courier
        };

        struct Job {
            Args     args;
            FWATCard result;
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

        uCondition        jobReady; // courier wait for jobs to become available
        std::queue<Job *> jobs;     // queue of jobs

        std::vector<Courier *> couriers;    // fixed size pool of couriers

    public:
        _Event Lost {};
        WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
        FWATCard create( unsigned int sid, unsigned int amount );
        FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
        Job *requestWork();
        ~WATCardOffice ();
};

#endif
