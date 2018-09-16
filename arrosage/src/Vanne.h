#pragma once

#include "Activite.h"

#include <Arduino.h>

namespace hpms {

   class Vanne : public json::IJSonData {
   public:

      static bool pin_est_valide( uint8_t pin );

   public:

      Vanne( void );

      Vanne( uint8_t p, const Activite & m, const Activite & s );

   public:

      const json::CoDec & getCoDec( void ) const;

   public:

      bool est( uint8_t pin ) const;

      void ouvrir( void );

      void fermer( void );

      void evaluer( const Instant & maintenant );

   private:

      static const uint8_t MAX_PIN = 8;

      uint8_t  pin;
      Activite matin;
      Activite soir;
      bool     ouverte;

   friend struct VanneCodec;
   };
}
