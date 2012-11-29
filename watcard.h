#ifndef __WATCARD_H__
#define __WATCARD_H__

#include <uFuture.h>

class WATCard {
   private:
      unsigned int balance;
      WATCard( const WATCard & );			// prevent copying
      WATCard &operator=( const WATCard & );
   public:
      WATCard();
      void deposit( unsigned int amount );
      void withdraw( unsigned int amount );
      unsigned int getBalance();
};
typedef Future_ISM<WATCard *> FWATCard;		// future WATCard pointer

#endif
