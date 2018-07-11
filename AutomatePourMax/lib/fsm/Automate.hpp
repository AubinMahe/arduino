#pragma once

#include "Automate_impl.hpp"

namespace fsm {

  template<class S, class E>
  class Automate : public Automate_impl {
  public:

     Automate( S etat_courant ) : Automate_impl((int)etat_courant ) {}

     void transition( S courant, E evenement, S futur, utils::action_t action = 0 ) {
        transition_((int)courant, (int)evenement, (int)futur, action );
     }

     bool evenement( utils::Acteur & acteur, E evenement ) {
        return evenement_( acteur, (int)evenement );
     }
  };
}
