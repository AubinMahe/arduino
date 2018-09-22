#pragma once

#include "Activite.h"

namespace hpms {

   class Vanne : public json::IJSonData {
   public:

      Vanne( void );

      Vanne( const Activite & m, const Activite & s );

   public:

      const json::CoDec & getCoDec( void ) const;

   public:

      void ouvrir( uint8_t pin );

      void fermer( uint8_t pin );

      void evaluer( uint8_t pin, const Instant & maintenant );

   private:

      Activite matin;
      Activite soir;
      bool     ouverte;

   friend struct VanneCodec;
   };
}
