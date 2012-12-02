#include <uC++.h>
#include "office.h"
#include "MPRNG.h"
#include "bank.h"
#include "printer.h"

extern MPRNG mprng;

// WATCardOffice
/*{{{*/
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) :
    prt(prt), bank(bank), numCouriers(numCouriers)
{
    prt.print( Printer::WATCardOffice, (char)WATCardOffice::Start );
}

WATCardOffice::~WATCardOffice ()
{
    for (int i=0; i<couriers.size(); i++) {
        delete couriers[i];
    }
    prt.print( Printer::WATCardOffice, (char)WATCardOffice::Finished );
}

// Create <numCouriers> couriers
void WATCardOffice::main()
{
    for (int i=0; i<numCouriers; i++) {
        Courier *c = new Courier(prt, *this, bank, i);
        couriers.push_back(c);
    }
}

FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount )
{
    Args args = {sid, amount, NULL};
    Job *j = new Job(args);
    jobs.push(j);
    jobReady.signal();
    prt.print( Printer::WATCardOffice, (char)WATCardOffice::CreationComplete, sid, amount );
    return j->result;
}

FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card )
{
    Args args = {sid, amount, card};
    Job *j = new Job(args);
    jobs.push(j);
    jobReady.signal();
    prt.print( Printer::WATCardOffice, (char)WATCardOffice::TransferComplete, sid, amount );
    return j->result;
}

WATCardOffice::Job* WATCardOffice::requestWork()
{
    if (jobs.size() == 0) jobReady.wait();
    prt.print( Printer::WATCardOffice, (char)WATCardOffice::CourierComplete );
    Job *j = jobs.front();
    jobs.pop();
    return j;
}
/*}}}*/

// WATCardOffice::Courier
/*{{{*/
WATCardOffice::Courier::Courier(Printer &prt, WATCardOffice& cardOffice, Bank& bank, unsigned int id) :
    prt(prt), office(cardOffice), bank(bank), id(id)
{
    prt.print( Printer::Courier, id, (char)WATCardOffice::Courier::Start );
}

WATCardOffice::Courier::~Courier()
{
    prt.print( Printer::Courier, id, (char)WATCardOffice::Courier::Finished );
}

void WATCardOffice::Courier::main()
{
    for (;;) {
        Job *j = office.requestWork();
        prt.print( Printer::Courier, id, (char)WATCardOffice::Courier::StartTransfer, j->args.sid, j->args.amount );

        // do work to transfer money
        WATCard *card = j->args.card == NULL ? new WATCard : j->args.card;
        // TODO communicate with bank
        bank.withdraw(j->args.sid, j->args.amount);
        card->deposit(j->args.amount);

        prt.print( Printer::Courier, id, (char)WATCardOffice::Courier::CompleteTransfer, j->args.sid, j->args.amount );
        // 1 in 6 chance of losing the WATCard, thereby also losing the money on that 
        // card
        if (mprng(6-1) == 0) {
            j->result.exception(new WATCardOffice::Lost);
            continue;
        }
        j->result.delivery(card);
    }
}
/*}}}*/
