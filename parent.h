#ifndef __PARENT_H__
#define __PARENT_H__

_Monitor Printer;
_Monitor Bank;

_Task Parent {
   private:
      void main();
   public:
      Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
};

#endif
