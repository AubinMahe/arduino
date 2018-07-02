#include <EvenementsTemporels_impl.hpp>
#include <Arduino.h>

struct EvenementTemporel {

   unsigned long       _delai;
   int                 _evenement;
   bool                _est_arme;
   EvenementTemporel * _suivant;

   EvenementTemporel( unsigned long delai, int evenement, EvenementTemporel * suivant ) :
      _delai    ( delai     ),
      _evenement( evenement ),
      _est_arme ( false     ),
      _suivant  ( suivant   )
   {}

   bool evalue( unsigned long temps_ecoule ) {
      if( _est_arme && temps_ecoule >= _delai ) {
         _est_arme = false;
         return true;
      }
      return false;
   }

   void desactive( void ) {
      _est_arme = false;
   }

   void arme( void ) {
      _est_arme = true;
   }

private:
   EvenementTemporel( const EvenementTemporel & );
};

EvenementsTemporels_impl::EvenementsTemporels_impl( Automate_impl & automate ) :
   _automate           ( automate ),
   _evenementsTemporels( 0        ),
   _t0                 ( 0        )
{}

void EvenementsTemporels_impl::ajoute_( unsigned long delai, int evenement ) {
   _evenementsTemporels = new EvenementTemporel( delai, evenement, _evenementsTemporels );
}

void EvenementsTemporels_impl::desactive( void ) {
   _t0 = 0;
   for( EvenementTemporel * et = _evenementsTemporels; et; et = et -> _suivant ) {
      et -> desactive();
   }
}

void EvenementsTemporels_impl::arme( void ) {
   _t0 = millis();
   for( EvenementTemporel * et = _evenementsTemporels; et; et = et -> _suivant ) {
      et -> arme();
   }
}

bool EvenementsTemporels_impl::evalue() {
   unsigned long temps_ecoule = millis() - _t0;
   for( EvenementTemporel * et = _evenementsTemporels; et; et = et -> _suivant ) {
      if( et -> evalue( temps_ecoule )) {
         _automate.evenement_( et -> _evenement );
      }
   }
   return false;
}

void EvenementsTemporels_impl::debug() const {
   for( EvenementTemporel * et = _evenementsTemporels; et; et = et -> _suivant ) {
      Serial.print( "EvenementsTemporels.debug|delai: " );
      Serial.print( et -> _delai );
      Serial.print( ", evenement: " );
      Serial.print( et -> _evenement );
      Serial.print( ", est_arme: " );
      Serial.println( et -> _est_arme );

   }
}
