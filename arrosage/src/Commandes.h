#pragma once

#include "Arrosage.h"

namespace hpms {

   struct Configuration : public json::IJSonData {

      Configuration( const Instant & i, const Arrosage & a ) :
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
}
