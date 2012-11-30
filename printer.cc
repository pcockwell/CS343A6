#include <uC++.h>
#include <iostream>
#include "q1printer.h"
#include "q1voter.h"

using namespace std;

//---------------------------------------------------------------------------------------
// Printer::Printer()
//
// Constructor for the printer
//---------------------------------------------------------------------------------------
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers ){
   this->nStudents = numStudents;
   this->nVendingMachines = numVendingMachines;
   this->nCouriers = numCouriers;

   //Total number of members is the number of students + number of machines
   // + number of couriers + the Parent, WATOffice, Nameserver, Truck, and Plant
   this->nMembers = this->nStudents + this->nVendingMachines + this->nCouriers + 5;

   //Create the states array
   this->userStates = new OutputState*[this->nMembers];

   int index = 0;

   this->userStates[index] = new OutputState;
   cout << "Parent";
   cout << "\t";
   index++;

   this->userStates[index] = new OutputState;
   cout << "WATOff";
   cout << "\t";
   index++;

   this->userStates[index] = new OutputState;
   cout << "Names";
   cout << "\t";
   index++;

   this->userStates[index] = new OutputState;
   cout << "Truck";
   cout << "\t";
   index++;

   this->userStates[index] = new OutputState;
   cout << "Plant";
   cout << "\t";
   index++;

   for ( unsigned int i = 0; i < this->nStudents; i++ ){
      this->userStates[index] = new OutputState;
      cout << "Stud" << i;
      cout << "\t";
      index++;
   }

   for ( unsigned int i = 0; i < this->nVendingMachines; i++ ){
      this->userStates[index] = new OutputState;
      cout << "Mach" << i;
      cout << "\t";
      index++;
   }

   for ( unsigned int i = 0; i < this->nCouriers; i++ ){
      this->userStates[index] = new OutputState;
      cout << "Cour" << i;
      cout << "\t";
      index++;
   }

   cout << endl;

   for ( unsigned int i = 0; i < this->nMembers; i++ ){
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

   for ( unsigned int i = 0; i < this->nMembers; i++ ){
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

   for ( unsigned int i = 0; i < this->nMembers; i++ ){
      
      cout << ( this->userStates[i]->state == "" ? "" : this->userStates[i]->state );
      if ( this->userStates[i]->val1 != -1 ){
         cout << this->userStates[i]->val1;
         if ( this->userStates[i]->val2 != -1 ){
            cout << "," << this->userStates[i]->val2;
         }
      }
      cout << "\t";

      this->userStates[i]->state = "";
      this->userStates[i]->val1 = -1;
      this->userStates[i]->val2 = -1;
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

   if ( state == 'F' || this->userStates[memberId]->state != "" ){
       this->flush();
   }

   this->userStates[memberId]->state += state;

   if ( state == 'F' ){
      for ( unsigned int i = 0; i < this->nMembers; i++ ){
         if ( memberId != i ){
            this->userStates[i]->state = "...";                
         }
      }
      this->flush();
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

   if ( this->userStates[memberId]->state != "" ){
       this->flush();
   }

   this->userStates[memberId]->state += state;
   this->userStates[memberId]->val1 = value1;
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

   if ( this->userStates[memberId]->state != "" ){
       this->flush();
   }

   this->userStates[memberId]->state += state;
   this->userStates[memberId]->val1 = value1;
   this->userStates[memberId]->val1 = value2;
}

void Printer::print( Kind kind, unsigned int lid, char state ){

   int memberId;

   switch(kind){
      case Student:
         memberId = 5 + lid;
         break;
      case Vending:
         memberId = 5 + this->nStudents + lid;
         break;
      case Courier:
         memberId = 5 + this->nStudents + this->nVendingMachines + lid;
         break;
   }

   if ( state == 'F' || this->userStates[memberId]->state != "" ){
       this->flush();
   }

   this->userStates[memberId]->state += state;

   if ( state == 'F' ){
      for ( unsigned int i = 0; i < this->nMembers; i++ ){
         if ( memberId != i ){
            this->userStates[i]->state = "...";                
         }
      }
      this->flush();
   }
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ){

   int memberId;

   switch(kind){
      case Student:
         memberId = 5 + lid;
         break;
      case Vending:
         memberId = 5 + this->nStudents + lid;
         break;
      case Courier:
         memberId = 5 + this->nStudents + this->nVendingMachines + lid;
         break;
   }

   if ( this->userStates[id]->state != "" ){
       this->flush();
   }

   this->userStates[memberId]->state += state;
   this->userStates[memberId]->val1 = value1;
}
void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ){   int memberId;

   switch(kind){
      case Student:
         memberId = 5 + lid;
         break;
      case Vending:
         memberId = 5 + this->nStudents + lid;
         break;
      case Courier:
         memberId = 5 + this->nStudents + this->nVendingMachines + lid;
         break;
   }

   if ( this->userStates[id]->state != "" ){
       this->flush();
   }

   this->userStates[memberId]->state += state;
   this->userStates[memberId]->val1 = value1;
   this->userStates[memberId]->val2 = value2;
}