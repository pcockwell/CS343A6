#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <uC++.h>
#include <vector>

_Monitor Printer {
   public:
      enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
      Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
      void print( Kind kind, char state );
      void print( Kind kind, char state, int value1 );
      void print( Kind kind, char state, int value1, int value2 );
      void print( Kind kind, unsigned int lid, char state );
      void print( Kind kind, unsigned int lid, char state, int value1 );
      void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
      ~Printer();

   private:
      // information about voter buffered by the printer
      struct buf_state {
         Voter::States  state;
         bool           vote;
      };

      unsigned int   numVoters;
      bool           needFlush;
      std::vector<buf_state>  buffer;
      std::vector<bool>       dirty;

      void flush_buffer();
      void flush_finish( unsigned int id );

      // helper to reduce code duplication
      //_Mutex void print( unsigned int id, Voter::States state, unsigned int numBlocked, bool vote);

};

#endif
