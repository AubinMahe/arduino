#pragma once

#include "Automate_impl.hpp"

namespace fsm {

  struct EvenementTemporel;
  class EvenementsTemporels_impl {
  public:

     EvenementsTemporels_impl( Automate_impl & automate );

     void desarme( void );

     void arme( void );

     void evalue( utils::Acteur & acteur );

     void debug( void ) const;

  protected:

     void ajoute_( unsigned long delai, int evenement );

  private:

     Automate_impl &     _automate;
     EvenementTemporel * _evenementsTemporels;
     unsigned long       _t0;

  private:
     EvenementsTemporels_impl( const EvenementsTemporels_impl & );
     EvenementsTemporels_impl & operator = ( const EvenementsTemporels_impl & );
  };
}
