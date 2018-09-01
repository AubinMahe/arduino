#pragma once

#include "Activite.h"

#include <Arduino.h>

namespace hpms {

   class Vanne {
   public:

      Vanne( void ) :
         pin( 0 ),
         ouverte( false )
      {}

      Vanne( uint8_t p, const Activite & m, const Activite & s ) :
         pin( p ),
         ouverte( false ),
         matin( m ),
         soir( s )
      {}

      void ouvrir( void );

      void fermer( void );

      void evaluer( const Instant & maintenant );

      /**
       * Analyse une chaine de catactères pour en extraire les informations
       * temporelles d'ouverture et de fermeture de vanne pour matin et soir.
       * @param hhsmm une chaine respectant le format hh:mm-hh:mm|hh:mm-hh:mm
       * @return vrai si la chaine respecte le format et que les informations
       * temporelles sont valides.
       */
      bool decode( char activite[] );

   private:

      static const uint8_t MAX_PIN = 8;

      uint8_t  pin;
      bool     ouverte;
      Activite matin;
      Activite soir;
   };
}
