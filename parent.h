#ifndef __PARENT_H__
#define __PARENT_H__

_Monitor Printer;
_Monitor Bank;

_Task Parent {
   private:

      enum States { Start = 'S', Deposit = 'D', Finished = 'F' };

      void main();
   public:
      Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif
