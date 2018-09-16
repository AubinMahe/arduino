#pragma once

#include "Arrosage.h"

namespace hpms {

   template<class A>
   struct Commande : public json::IJSonData {

      Commande() {
         /**/
      }

      char commande[30];
      A    argument;

      struct CoDec : public json::CoDec {
         CoDec() :
            json::CoDec(
               new json::String( "commande", &Commande::commande,
               new json::Object( "argument", &Commande::argument )))
         {}
      } codec;

      virtual const json::CoDec & getCoDec() const { return codec; }
   };

   struct Configuration : public json::IJSonData {

      Configuration( const Instant & i = Instant(), const Arrosage & a = Arrosage()) :
         heure( i ),
         arrosage( a )
      {}

      virtual const json::CoDec & getCoDec() const;

      Instant  heure;
      Arrosage arrosage;
   };

   struct DemarrerArreter : public json::IJSonData {

      DemarrerArreter() :
         demarrer( false )
      {}

      virtual const json::CoDec & getCoDec() const;

      bool demarrer;
   };

   struct CommanderUneVanne : public json::IJSonData {

      CommanderUneVanne() :
         pin( 0 ),
         ouvrir( false )
      {}

      virtual const json::CoDec & getCoDec() const;

      uint8_t pin;
      bool    ouvrir;
   };

   struct CommanderLesVannes : public json::IJSonData {

      virtual const json::CoDec & getCoDec() const;

      CommanderUneVanne les_vannes[100];
   };
}
