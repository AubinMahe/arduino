#pragma once

#include <ESP8266WiFi.h>

#include "Horloge.h"
#include "Vanne.h"

namespace hpms {

   class Arrosage : private json::IJSonData {
   public:

      Arrosage();

   public:

      void demarrer( bool demarrer );

      void commander_une_vanne( uint8_t pin, bool ouvrir );

      void evaluer( Horloge & horloge );

   public:

      json::Status decode( const char * name, json::Decoder & decoder );

      json::Status encode( json::Encoder & encoder ) const;

   private:

      bool  est_en_marche;
      Vanne vannes[4];
   };
}
