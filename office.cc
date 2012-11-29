#include <uC++.h>
#include "office.h"

// WATCardOffice
/*{{{*/
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers )
   : printer(prt), bank(bank), numCouriers(numCouriers)
{
}

WATCardOffice::~WATCardOffice ()
{
   for (int i=0; i<couriers.size(); i++) {
      delete couriers[i];
   }
}

// Create <numCouriers> couriers
void WATCardOffice::main()
{
   for (int i=0; i<numCouriers; i++) {
      Courier *c = new Courier(*this);
      couriers.push_back(c);
   }
}

FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount )
{
   Args args = {sid, amount, NULL};
   Job *j = new Job(args);
   jobs.push(j);
   jobReady.signal();
   return j->result;
}

FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card )
{
   Args args = {sid, amount, card};
   Job *j = new Job(args);
   jobs.push(j);
   jobReady.signal();
   return j->result();
}

WATCardOffice::Job* WATCardOffice::requestWork()
{
   if (jobs.size() == 0) jobReady.wait();
   Job *j = jobs.front();
   jobs.pop();
   return j;
}
/*}}}*/

// WATCardOffice::Courier
/*{{{*/
WATCardOffice::Courier::Courier(WATCardOffice& cardOffice)
   :office(watcardOffice)
{
}

void WATCardOffice::Courier::main()
{
   for (;;) {
      Job *j = office.requestWork();

      // do work to transfer money
      WATCard *card = j->args.card == NULL ? new WATCard : j->args.card;
      // TODO communicate with bank

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
