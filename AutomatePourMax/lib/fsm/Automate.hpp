#pragma once

#include <Arduino.h>
#include <Automate_impl.hpp>
#include <EvenementsTemporels_impl.hpp>

template<class S, class E>
class Automate : public Automate_impl {
public:

   Automate( S etat_courant ) : Automate_impl((int)etat_courant ) {}

   void transition( S courant, E evenement, S futur, Action_t action = 0 ) {
      transition_((int)courant, (int)evenement, (int)futur, action );
   }

   bool evenement( E evenement ) {
      return evenement_((int)evenement );
   }
};

template<class S, class E>
class EvenementsTemporels : public EvenementsTemporels_impl {
public:

   EvenementsTemporels( Automate<S, E> & automate ) :
      EvenementsTemporels_impl( automate )
   {}

   void ajoute( unsigned long delai, E evenement ) {
      ajoute_( delai, (int)evenement );
   }
};
