#pragma once

#include "Horloge.h"
#include "Vanne.h"

namespace hpms {

   class Arrosage : public json::IJSonData {
   public:

      static const uint8_t NBR_VANNES = 4;

      Arrosage();

   public:

      const json::CoDec & getCoDec( void ) const;

   public:

      void demarrer( bool demarrer );

      void commander_une_vanne( uint8_t pin, bool ouvrir );

      void evaluer( Horloge & horloge );

   private:

      bool  est_en_marche;
      Vanne vannes[NBR_VANNES];

   friend struct ArrosageCoDec;
   };
}
