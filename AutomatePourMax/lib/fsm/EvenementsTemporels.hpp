#pragma once

#include "EvenementsTemporels_impl.hpp"
#include "Automate.hpp"

namespace fsm {

  template<class S, class E>
  class EvenementsTemporels : public fsm::EvenementsTemporels_impl {
  public:

     EvenementsTemporels( Automate<S, E> & automate ) :
        EvenementsTemporels_impl( automate )
     {}

     void ajoute( unsigned long delai, E evenement ) {
        ajoute_( delai, (int)evenement );
     }
  };
}
