#pragma once

#include <ESP8266WiFi.h>

#include "Horloge.h"
#include "Vanne.h"

namespace hpms {

   class Arrosage : public json::IJSonData {
   public:

      Arrosage();

   public:

      const json::CoDec & getCoDec( void ) const;

   public:

      void demarrer( bool demarrer );

      void commander_une_vanne( uint8_t pin, bool ouvrir );

      void evaluer( Horloge & horloge );

   private:

      bool  est_en_marche;
      Vanne vannes[4];

   friend struct ArrosageCoDec;
   };
}
