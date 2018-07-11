#include "EvenementsTemporels_impl.hpp"

#include <Arduino.h>

namespace fsm {

  struct EvenementTemporel {

    const unsigned long       _delai;
    const int                 _evenement;
    /* */ EvenementTemporel * _suivant;
    /* */ bool                _est_arme;

    EvenementTemporel(
      unsigned long       delai,
      int                 evenement,
      EvenementTemporel * suivant  )
       :
      _delai    ( delai     ),
      _evenement( evenement ),
      _suivant  ( suivant   ),
      _est_arme ( false     )
    {}

    bool evalue(unsigned long temps_ecoule) {
      if( _est_arme && temps_ecoule >= _delai ) {
        _est_arme = false;
        return true;
      }
      return false;
    }

    void desarme(void) { _est_arme = false; }

    void arme(void) { _est_arme = true; }

  private:
    EvenementTemporel(const EvenementTemporel &);
  };
}

using namespace fsm;

EvenementsTemporels_impl::EvenementsTemporels_impl( Automate_impl & automate ) :
  _automate           ( automate ),
  _evenementsTemporels( 0        ),
  _t0                 ( 0        )
{}

void EvenementsTemporels_impl::ajoute_( unsigned long delai, int evenement ) {
  _evenementsTemporels =
    new EvenementTemporel( delai, evenement, _evenementsTemporels );
}

void EvenementsTemporels_impl::desarme(void) {
  _t0 = 0;
  for( EvenementTemporel * et = _evenementsTemporels; et; et = et->_suivant ) {
    et->desarme();
  }
}

void EvenementsTemporels_impl::arme(void) {
  _t0 = millis();
  for( EvenementTemporel * et = _evenementsTemporels; et; et = et->_suivant ) {
    et->arme();
  }
}

void EvenementsTemporels_impl::evalue(utils::Acteur &acteur) {
  const unsigned long temps_ecoule = millis() - _t0;
  for( EvenementTemporel * et = _evenementsTemporels; et; et = et->_suivant ) {
    if( et->evalue( temps_ecoule )) {
      _automate.evenement_( acteur, et->_evenement );
    }
  }
}

void EvenementsTemporels_impl::debug() const {
  for (EvenementTemporel *et = _evenementsTemporels; et; et = et->_suivant) {
    Serial.print("EvenementsTemporels.debug|delai: ");
    Serial.print(et->_delai);
    Serial.print(", evenement: ");
    Serial.print(et->_evenement);
    Serial.print(", est_arme: ");
    Serial.println(et->_est_arme);
  }
}
