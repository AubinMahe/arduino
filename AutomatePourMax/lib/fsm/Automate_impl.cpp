#include "Automate_impl.hpp"

#include <Arduino.h>

namespace fsm {

   struct Automate_Transition {
      Automate_Transition( int                  courant,
                           int                  evenement,
                           int                  futur,
                           utils::action_t      action,
                           Automate_Transition* suivante )
         : _etat_courant( courant )
         , _evenement( evenement )
         , _futur_etat( futur )
         , _action( action )
         , _suivante( suivante ) {}

      int                  _etat_courant;
      int                  _evenement;
      int                  _futur_etat;
      utils::action_t      _action;
      Automate_Transition* _suivante;
   };
}

using namespace fsm;

Automate_impl::Automate_impl( int etat_courant )
   : _traceEvenement( false )
   , _traceEtat( false )
   , _etat_courant( etat_courant )
   , _transitions( 0 ) {}

void Automate_impl::transition_( int courant, int evenement, int futur, utils::action_t action ) {
   _transitions = new Automate_Transition( courant, evenement, futur, action, _transitions );
}

bool Automate_impl::evenement_( utils::Acteur& acteur, int evenement ) {
   if( _traceEvenement ) {
      Serial.print( "Automate_impl::evenement_|evenement: " );
      Serial.println( evenement );
   }
   Automate_Transition* t = _transitions;
   while( t ) {
      if( t->_etat_courant == _etat_courant && t->_evenement == evenement ) {
         _etat_courant = t->_futur_etat;
         Serial.print( "passage dans l'état " );
         Serial.println( _etat_courant );
         Serial.flush();
         if( t->_action ) {
            ( acteur.*( t->_action ) )();
         }
         return true;
      }
      t = t->_suivante;
   }
   if( _traceEvenement ) {
      Serial.println( "Automate_impl::evenement_|evenement ignore." );
   }
   return false;
}

void Automate_impl::debug() const {
   for( Automate_Transition* t = _transitions; t; t = t->_suivante ) {
      Serial.print( "Automate.debug|" );
      Serial.print( t->_etat_courant );
      Serial.print( " + " );
      Serial.print( t->_evenement );
      Serial.print( " ==> " );
      Serial.println( t->_futur_etat );
   }
   Serial.print( "Automate.debug|etat courant: " );
   Serial.println( _etat_courant );
   Serial.flush();
}
