#pragma once

#include <ESP8266WiFi.h>

#include "Horloge.h"
#include "Vanne.h"

namespace hpms {

   class Arrosage {
   public:

      Arrosage();

   public:

      void demarrer( bool demarrer );

      void commander_une_vanne( uint8_t pin, bool ouvrir );

      void evaluer( Horloge & horloge );

   public:

      json::Status decode( const char * name, json::Parser & parser );

      json::Status encode( json::Generator & generator ) const;

   private:

      bool  est_en_marche;
      Vanne vannes[4];
   };
}
