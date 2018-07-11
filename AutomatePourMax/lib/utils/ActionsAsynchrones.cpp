#include "ActionsAsynchrones.hpp"

#include <Arduino.h> // millis(), Serial

namespace utils {

   struct Action {

      Action( void ) : _consigne( 0UL ), _t0( 0UL ), _cycle( 0UL ), _action( 0 ) {}

      virtual ~ Action( void ) {}

      inline bool estDisponible( action_t action ) const {
         return _action == 0 || _action == action;
      }

      inline void mise_a_jour( action_t action, unsigned long consigne ) {
         _consigne = consigne;
         _t0       = millis();
         _cycle    = 0UL;
         _action   = action;
         // print( "Action::mise_a_jour" );
      }

      inline void print( const char* title ) {
         Serial.print( millis() );
         Serial.print( ": " );
         Serial.print( title );
         Serial.print( "|consigne: " );
         Serial.print( _consigne );
         Serial.print( ", t0: " );
         Serial.print( _t0 );
         Serial.print( ", cycle: " );
         Serial.print( _cycle );
         Serial.print( ", action:" );
         Serial.print( (unsigned long)(const void*)_action );
         Serial.println();
         Serial.flush();
      }

      inline void desactive( void ) {
         // print( "Action::desactive" );
         _consigne = 0;
         _t0       = 0;
         _cycle    = 0;
         _action   = 0;
      }

      virtual bool estActivable( void ) const = 0;

      virtual void active( Acteur& acteur ) = 0;

      unsigned long _consigne;
      unsigned long _t0;
      unsigned long _cycle;
      action_t      _action;
   };

   struct ActionPeriodique : public Action {

      inline ActionPeriodique( action_t action, unsigned long periode_ms ) {
         mise_a_jour( action, periode_ms );
      }

      virtual inline bool estActivable( void ) const {
         if( _action && _consigne ) {
            unsigned long temps_ecoule = millis() - _t0;
            if( ( temps_ecoule / _consigne ) > _cycle ) {
               return true;
            }
         }
         return false;
      }

      virtual inline void active( Acteur& acteur ) {
         ( acteur.*_action )();
         unsigned long temps_ecoule = millis() - _t0;
         _cycle                     = temps_ecoule / _consigne;
      }
   };

   struct ActionDifferee : public Action {

      inline ActionDifferee( action_t action, unsigned long delai_ms ) {
         mise_a_jour( action, delai_ms );
      }

      virtual inline bool estActivable( void ) const {
         if( _action && _consigne ) {
            return ( ( millis() - _t0 ) >= _consigne );
         }
         return false;
      }

      virtual inline void active( Acteur& acteur ) {
         ( acteur.*_action )();
         desactive();
      }
   };
} // namespace utils

using namespace utils;

const unsigned ActionsAsynchrones::CAPACITE_DIFFEREES;
const unsigned ActionsAsynchrones::CAPACITE_PERIODIQUES;

ActionsAsynchrones::ActionsAsynchrones( void ) {
   memset( _actions, 0, sizeof( _actions ) );
}

bool ActionsAsynchrones::differee( action_t a, unsigned long delai_ms ) {
   for( unsigned i = 0; i < sizeof( _actions ) / sizeof( _actions[ 0 ] ); ++i ) {
      Action* action = _actions[ i ];
      if( action == 0 ) {
         _actions[ i ] = new ActionDifferee( a, delai_ms );
         return true;
      }
      if( action->estDisponible( a ) ) {
         action->mise_a_jour( a, delai_ms );
         return true;
      }
   }
   Serial.println( "ActionsAsynchrones::differee|Impossible d'ajouter une action." );
   Serial.flush();
   return false;
}

bool ActionsAsynchrones::periodique( action_t a, unsigned long periode_ms ) {
   for( unsigned i = 0; i < sizeof( _actions ) / sizeof( _actions[ 0 ] ); ++i ) {
      Action* action = _actions[ i ];
      if( action == 0 ) {
         _actions[ i ] = new ActionPeriodique( a, periode_ms );
         return true;
      }
      if( action->estDisponible( a ) ) {
         action->mise_a_jour( a, periode_ms );
         return true;
      }
   }
   Serial.println( "ActionsAsynchrones::periodique|Impossible d'ajouter une action." );
   Serial.flush();
   return false;
}

bool ActionsAsynchrones::desactive( action_t a ) {
   for( unsigned i = 0; i < sizeof( _actions ) / sizeof( _actions[ 0 ] ); ++i ) {
      Action* action = _actions[ i ];
      if( action->_action == a ) {
         action->desactive();
         return true;
      }
   }
   return false;
}

void ActionsAsynchrones::evalue( Acteur& acteur ) {
   for( unsigned i = 0; i < sizeof( _actions ) / sizeof( _actions[ 0 ] ); ++i ) {
      Action* action = _actions[ i ];
      if( action && action->estActivable() ) {
         action->active( acteur );
      }
   }
}
