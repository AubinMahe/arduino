#include "ActionsDifferees.hpp"

#include <Arduino.h>       // millis()

class ActionDifferee {
public:

   ActionDifferee(
      unsigned long                     t0,
      ActionsDifferees::actionDiffere_t action,
      unsigned long                     delai,
      ActionDifferee *                  suivante  )
    :
      _t0      ( t0       ),
      _action  ( action   ),
      _delai   ( delai    ),
      _suivante( suivante )
   {}

   inline bool estActivable() const {
      unsigned long temps_ecoule = millis() - _t0;
      return temps_ecoule >= _delai;
   }

   unsigned long                     _t0;
   ActionsDifferees::actionDiffere_t _action;
   unsigned long                     _delai;
   ActionDifferee *                  _suivante;

private:
   ActionDifferee( const ActionDifferee & );
   ActionDifferee & operator = ( const ActionDifferee & );
};

ActionsDifferees::ActionsDifferees() :
   _actions( 0 )
{}

void ActionsDifferees::ajoute( ActionsDifferees::actionDiffere_t action, unsigned long delay ) {
   _actions = new ActionDifferee( millis(), action, delay, _actions );
}

void ActionsDifferees::evalue( void ) {
   ActionDifferee * precedente = _actions;
   for( ActionDifferee * ad = _actions; ad; ad = ad -> _suivante ) {
      if( ad -> estActivable()) {
         ad -> _action();
         precedente -> _suivante = ad -> _suivante;
         delete ad;
      }
      precedente = ad;
   }
}
