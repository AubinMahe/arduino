#pragma once

#include "Acteur.hpp"

namespace utils {

   struct Action;
   class ActionsAsynchrones {
   public:
      static const unsigned CAPACITE_DIFFEREES   = 5;
      static const unsigned CAPACITE_PERIODIQUES = 5;

      ActionsAsynchrones( void );

      bool differee( action_t action, unsigned long delai_ms );

      bool periodique( action_t action, unsigned long periode_ms );

      bool desactive( action_t action );

      void evalue( Acteur& acteur );

   private:
      Action* _actions[ CAPACITE_DIFFEREES + CAPACITE_PERIODIQUES ];

   private:
      ActionsAsynchrones( const ActionsAsynchrones& );
      ActionsAsynchrones& operator=( const ActionsAsynchrones& );
   };
} // namespace utils
