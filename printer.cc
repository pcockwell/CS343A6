#include <uC++.h>
#include <iostream>
#include "q1printer.h"
#include "q1voter.h"

using namespace std;

/*
 * Printer header
 */
Printer::Printer( unsigned int voters )
   : numVoters(voters), needFlush(false), dirty(voters, false)
{
   buffer.reserve(numVoters);
   for (unsigned int i=0; i<voters; i++) {
      cout << "Voter" << i << "\t";
   }
   cout << endl;
   for (unsigned int i=0; i<voters; i++) {
      cout << "=======" << "\t";
   }
   cout << endl;
}

/*
 * Flush buffer if not terminated properly.
 * Printer footer
 */
Printer::~Printer()
{
   if (needFlush) {
      flush_buffer();
   }
   cout << "=================" << endl;
   cout << "All tours started" << endl;
}

void Printer::flush_buffer()
{
   if (!needFlush) return; // do not flush at all if nothing new was buffered
   for (unsigned int i=0; i<numVoters; i++) {
      if (!dirty[i]) {
         cout << '\t';     // empty column if nothing new was buffered
         continue;
      }
      switch(buffer[i].state) {
         case Voter::Start:
         case Voter::Complete:
            cout << (char)buffer[i].state << "\t";
            break;
         case Voter::Vote:
         case Voter::Finished:
            cout << (char)buffer[i].state << (buffer[i].vote ? " 1\t" : " 0\t");
            break;
         case Voter::Block:
         case Voter::Unblock:
            cout << (char)buffer[i].state << " " << buffer[i].numBlocked << "\t";
            break;
         default:
            exit(EXIT_FAILURE);
      }
      dirty[i] = false;
   }
   cout << endl;
   needFlush = false;
}

/*
 * Flush buffer when a party finishes
 */
void Printer::flush_finish(unsigned int id)
{
   for (unsigned int i=0; i<numVoters; i++) {
      if (i == id) cout << (char)Voter::Finished << (buffer[i].vote ? " 1\t" : " 0\t");
      else cout << "...\t";
      dirty[i] = false;
   }
   cout << endl;
   needFlush = false;
}

/*
 * Flush buffer
 */
void Printer::print( unsigned int id, Voter::States state )
{
   print(id, state, 0, false);
}

/*
 * Flush buffer
 */
void Printer::print( unsigned int id, Voter::States state, bool vote )
{
   print(id, state, 0, vote);
}

/*
 * Flush buffer
 */
void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked )
{
   print(id, state, numBlocked, false);
}

/*
 * Flush buffer
 */
void Printer::print( unsigned int id, Voter::States state, unsigned int numBlocked, bool vote )
{
   if (dirty[id] || state == Voter::Finished) flush_buffer();
   buf_state bs = {id, state, numBlocked, vote};
   buffer[id] = bs;
   dirty[id] = true;
   needFlush = true;
   if (state == Voter::Finished) flush_finish(id);
}
