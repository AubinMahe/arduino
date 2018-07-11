#pragma once

#include <Acteur.hpp>

namespace fsm {

  struct Automate_Transition;
  class Automate_impl {
  protected:

     Automate_impl( int etat_courant );

     void transition_( int courant, int evenement, int futur, utils::action_t action );

  public:

     bool evenement_( utils::Acteur & acteur, int evenement );

     void debug() const;

     bool _traceEvenement;
     bool _traceEtat;

  private:

     int                   _etat_courant;
     Automate_Transition * _transitions;

     Automate_impl( const Automate_impl & );
     Automate_impl & operator = ( const Automate_impl & );
  };
}
