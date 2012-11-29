#ifndef __STUDENT_H__
#define __STUDENT_H__

_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;

_Task Student {
   private:
      Printer&       printer;
      NameServer&    nameServer;
      WATCardOffice& office;
      unsigned int   id;
      unsigned int   maxPurchases;

      void main();
   public:
      Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
            unsigned int maxPurchases );
};
#endif
