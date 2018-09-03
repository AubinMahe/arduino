#pragma once

#include "Activite.h"

#include <Arduino.h>

namespace hpms {

   class Vanne : private json::IJSonData {
   public:

      static bool pin_est_valide( uint8_t pin );

   public:

      Vanne( void );

      Vanne( uint8_t p, const Activite & m, const Activite & s );

   public:

      bool est( uint8_t pin ) const;

      void ouvrir( void );

      void fermer( void );

      void evaluer( const Instant & maintenant );

   public:

      json::Status decode( const char * name, json::Decoder & decoder );

      json::Status encode( json::Encoder & encoder ) const;

   private:

      static const uint8_t MAX_PIN = 8;

      uint8_t  pin;
      bool     ouverte;
      Activite matin;
      Activite soir;
   };
}
