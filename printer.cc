#include <uC++.h>
#include <iostream>
#include "printer.h"

using namespace std;

//---------------------------------------------------------------------------------------
// Printer::Printer()
//
// Constructor for the printer
//---------------------------------------------------------------------------------------
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers )
{
    numStudents = numStudents;
    numVendingMachines = numVendingMachines;
    numCouriers = numCouriers;

    //Total number of members is the number of students + number of machines
    // + number of couriers + the Parent, WATOffice, Nameserver, Truck, and Plant
    numMembers = numStudents + numVendingMachines + numCouriers + 5;

    //Create the states array
    userStates = new OutputState*[numMembers];

    int index = 0;

    userStates[index] = new OutputState;
    cout << "Parent";
    cout << "\t";
    index++;

    userStates[index] = new OutputState;
    cout << "WATOff";
    cout << "\t";
    index++;

    userStates[index] = new OutputState;
    cout << "Names";
    cout << "\t";
    index++;

    userStates[index] = new OutputState;
    cout << "Truck";
    cout << "\t";
    index++;

    userStates[index] = new OutputState;
    cout << "Plant";
    cout << "\t";
    index++;

    for ( unsigned int i = 0; i < numStudents; i++ ){
        userStates[index] = new OutputState;
        cout << "Stud" << i;
        cout << "\t";
        index++;
    }

    for ( unsigned int i = 0; i < numVendingMachines; i++ ){
        userStates[index] = new OutputState;
        cout << "Mach" << i;
        cout << "\t";
        index++;
    }

    for ( unsigned int i = 0; i < numCouriers; i++ ){
        userStates[index] = new OutputState;
        cout << "Cour" << i;
        cout << "\t";
        index++;
    }

    cout << endl;

    for ( unsigned int i = 0; i < numMembers; i++ ){
        cout << "*******";
        cout << "\t";
    }
    cout << endl;

}

//---------------------------------------------------------------------------------------
// Printer::~Printer()
//
// Destructor for the printer
//---------------------------------------------------------------------------------------
Printer::~Printer(){
    cout << "***********************" << endl;

    for ( unsigned int i = 0; i < numMembers; i++ ){
        delete userStates[i];
    }

    delete userStates;
}

//---------------------------------------------------------------------------------------
// Printer::flush()
//
// Call to flush the buffer of all print info
//---------------------------------------------------------------------------------------
void Printer::flush(){

    for ( unsigned int i = 0; i < numMembers; i++ ){

        cout << ( userStates[i]->state == "" ? "" : userStates[i]->state );
        if ( userStates[i]->val1 != -1 ){
            cout << userStates[i]->val1;
            if ( userStates[i]->val2 != -1 ){
                cout << "," << userStates[i]->val2;
            }
        }
        cout << "\t";

        userStates[i]->state = "";
        userStates[i]->val1 = -1;
        userStates[i]->val2 = -1;
    }
    cout << endl;
}

void Printer::print( Kind kind, char state ){

    int memberId;

    switch(kind){
        case Parent:
            memberId = 0;
            break;
        case WATCardOffice:
            memberId = 1;
            break;
        case NameServer:
            memberId = 2;
            break;
        case Truck:
            memberId = 3;
            break;
        case BottlingPlant:
            memberId = 4;
            break;
    }

    if ( state == 'F' || userStates[memberId]->state != "" ){
        flush();
    }

    userStates[memberId]->state += state;

    if ( state == 'F' ){
        for ( unsigned int i = 0; i < numMembers; i++ ){
            if ( memberId != i ){
                userStates[i]->state = "...";                
            }
        }
        flush();
    }
}

void Printer::print( Kind kind, char state, int value1 ){

    int memberId;

    switch(kind){
        case Parent:
            memberId = 0;
            break;
        case WATCardOffice:
            memberId = 1;
            break;
        case NameServer:
            memberId = 2;
            break;
        case Truck:
            memberId = 3;
            break;
        case BottlingPlant:
            memberId = 4;
            break;
    }

    if ( userStates[memberId]->state != "" ){
        flush();
    }

    userStates[memberId]->state += state;
    userStates[memberId]->val1 = value1;
}

void Printer::print( Kind kind, char state, int value1, int value2 ){

    int memberId;

    switch(kind){
        case Parent:
            memberId = 0;
            break;
        case WATCardOffice:
            memberId = 1;
            break;
        case NameServer:
            memberId = 2;
            break;
        case Truck:
            memberId = 3;
            break;
        case BottlingPlant:
            memberId = 4;
            break;
    }

    if ( userStates[memberId]->state != "" ){
        flush();
    }

    userStates[memberId]->state += state;
    userStates[memberId]->val1 = value1;
    userStates[memberId]->val1 = value2;
}

void Printer::print( Kind kind, unsigned int lid, char state ){

    int memberId;

    switch(kind){
        case Student:
            memberId = 5 + lid;
            break;
        case Vending:
            memberId = 5 + numStudents + lid;
            break;
        case Courier:
            memberId = 5 + numStudents + numVendingMachines + lid;
            break;
    }

    if ( state == 'F' || userStates[memberId]->state != "" ){
        flush();
    }

    userStates[memberId]->state += state;

    if ( state == 'F' ){
        for ( unsigned int i = 0; i < numMembers; i++ ){
            if ( memberId != i ){
                userStates[i]->state = "...";                
            }
        }
        flush();
    }
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ){

    int memberId;

    switch(kind){
        case Student:
            memberId = 5 + lid;
            break;
        case Vending:
            memberId = 5 + numStudents + lid;
            break;
        case Courier:
            memberId = 5 + numStudents + numVendingMachines + lid;
            break;
    }

    if ( userStates[lid]->state != "" ){
        flush();
    }

    userStates[memberId]->state += state;
    userStates[memberId]->val1 = value1;
}
void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ){   int memberId;

    switch(kind){
        case Student:
            memberId = 5 + lid;
            break;
        case Vending:
            memberId = 5 + numStudents + lid;
            break;
        case Courier:
            memberId = 5 + numStudents + numVendingMachines + lid;
            break;
    }

    if ( userStates[lid]->state != "" ){
        flush();
    }

    userStates[memberId]->state += state;
    userStates[memberId]->val1 = value1;
    userStates[memberId]->val2 = value2;
}
