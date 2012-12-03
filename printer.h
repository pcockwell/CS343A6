#ifndef __PRINTER_H__
#define __PRINTER_H__

#include <uC++.h>
#include <iostream>

using namespace std;

//---------------------------------------------------------------------------------------
// Printer
//
// Printer related functions and functionality
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// Printer Monitor Definition
//
// Class definition for Printer
//---------------------------------------------------------------------------------------
_Monitor Printer {
    private:
        unsigned int numStudents;
        unsigned int numVendingMachines;
        unsigned int numCouriers;
        unsigned int numMembers;

        bool userStateChanged;

        struct OutputState{
            OutputState() : state(""), val1(-1), val2(-1){};

            string state;
            int val1;
            int val2;
        };

        OutputState** userStates;
        void flush();
    public:
        enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };

        Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
        ~Printer();

        void print( Kind kind, char state );
        void print( Kind kind, char state, int value1 );
        void print( Kind kind, char state, int value1, int value2 );
        void print( Kind kind, unsigned int lid, char state );
        void print( Kind kind, unsigned int lid, char state, int value1 );
        void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
}; // Printer

#endif //__PRINTER_H__
