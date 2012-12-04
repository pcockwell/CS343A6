#include <uC++.h>
#include "office.h"
#include "MPRNG.h"
#include "bank.h"
#include "printer.h"

extern MPRNG mprng;

// WATCardOffice
/*{{{*/
// Constructor
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) :
    prt(prt), bank(bank), numCouriers(numCouriers)
{
    prt.print( Printer::WATCardOffice, (char)WATCardOffice::Start );
}

// Destructor
WATCardOffice::~WATCardOffice ()
{
    // wait for couriers to finish
    for (int i=0; i<couriers.size(); i++) {
        delete couriers[i];
    }
    // free any remaining jobs
    while( jobs.size() != 0 ){
        Job *j = jobs.front();
        jobs.pop();
        delete j;
    }
    prt.print( Printer::WATCardOffice, (char)WATCardOffice::Finished );
}

void WATCardOffice::main()
{
    // Creates <numCouriers> couriers
    for (int i=0; i<numCouriers; i++) {
        Courier *c = new Courier(prt, *this, bank, i);
        couriers.push_back(c);
    }

    while( true ) {
        // If office is to terminate, send "terminating" jobs to couriers to 
        // terminate them first
        _Accept( ~WATCardOffice ){
            for (int i=0; i<couriers.size(); i++) {
                Args args = {0, 0, NULL, true};
                Job *j = new Job(args);
                jobs.push(j);
                jobReady.signal();
            }
            break;
        } or _Accept( create, transfer, requestWork ) {

        }
    }
}

// Creates a new watcard with a specified balance for a student
FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount )
{
    // Prepare arguments
    Args args = {sid, amount, new WATCard, false};

    // Create new job
    Job *j = new Job(args);
    jobs.push(j);

    // Signal couriers
    jobReady.signal();
    prt.print( Printer::WATCardOffice, (char)WATCardOffice::CreationComplete, sid, amount );

    // Return future
    return j->result;
}

// Transfer a specified amount from a student's bank account to his watcard
FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card )
{
    // Prepare arguments
    Args args = {sid, amount, card, false};

    // Create new job
    Job *j = new Job(args);
    jobs.push(j);

    // Signal couriers
    jobReady.signal();
    prt.print( Printer::WATCardOffice, (char)WATCardOffice::TransferComplete, sid, amount );

    // Return future
    return j->result;
}

// Courier calls to request a job
WATCardOffice::Job* WATCardOffice::requestWork()
{
    // If no job available, wait
    if (jobs.size() == 0) jobReady.wait();

    // Grab a job
    Job *j = jobs.front();
    if ( !j->args.termination ) {
        prt.print( Printer::WATCardOffice, (char)WATCardOffice::CourierComplete );
    }
    jobs.pop();

    // Courier responsible for deleting this job
    return j;
}
/*}}}*/

// WATCardOffice::Courier
/*{{{*/
// Constructor
WATCardOffice::Courier::Courier(Printer &prt,
                                WATCardOffice& cardOffice,
                                Bank& bank,
                                unsigned int id) :
    prt(prt), office(cardOffice), bank(bank), id(id)
{
    prt.print( Printer::Courier, id, (char)WATCardOffice::Courier::Start );
}

// Destructor
WATCardOffice::Courier::~Courier()
{
    prt.print( Printer::Courier, id, (char)WATCardOffice::Courier::Finished );
}

void WATCardOffice::Courier::main()
{
    for (;;) {
        // Grab a job, if it's a terminating job, stop
        Job *j = office.requestWork();
        if (j->args.termination) {
            delete j;
            break;
        }
        prt.print( Printer::Courier, id,
                (char)WATCardOffice::Courier::StartTransfer,
                j->args.sid, j->args.amount );

        // communicate with bank to transfer money
        WATCard *card = j->args.card;
        bank.withdraw(j->args.sid, j->args.amount);
        card->deposit(j->args.amount);

        prt.print( Printer::Courier, id,
                (char)WATCardOffice::Courier::CompleteTransfer,
                j->args.sid, j->args.amount );

        // 1 in 6 chance of losing the WATCard, thereby also losing the money on that 
        // card, raising an exception
        if (mprng(6-1) == 0) {
            delete j->args.card;
            j->result.exception(new WATCardOffice::Lost);
            delete j;
            continue;
        }
        // insert result into uFuture
        j->result.delivery(card);
        delete j;
    }
}
/*}}}*/
