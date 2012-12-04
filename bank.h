#ifndef __BANK_H__
#define __BANK_H__

_Monitor Bank {
    private:
        unsigned int numStudents;

        uCondition* funds;  // a condition for each student account
        unsigned int* accountBalances;  // a list of balances
    public:
        Bank( unsigned int numStudents );
        ~Bank();

        void deposit( unsigned int id, unsigned int amount );
        void withdraw( unsigned int id, unsigned int amount );
};

#endif
