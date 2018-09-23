#pragma once

#include "Activite.h"

namespace hpms {

   class Vanne : public json::IJSonData {
   public:

      enum Etat {
         PREMIER_ETAT = 1,

         CONFIGUREE_OUVERTE = PREMIER_ETAT,
         CONFIGUREE_FERMEE  = 2,
         FORCEE_OUVERTE     = 3,
         FORCEE_FERMEE      = 4,

         DERNIER_ETAT = FORCEE_FERMEE,
      };

      Vanne( void );

      Vanne( const Activite & m, const Activite & s );

   public:

      const json::CoDec & getCoDec( void ) const;

   public:

      void forcer_l_etat( uint8_t pin, const Instant & maintenant, Etat etat );

      void evaluer( uint8_t pin, const Instant & maintenant );

   private:

      void ouvrir ( uint8_t pin, const Instant & maintenant );
      void fermer ( uint8_t pin, const Instant & maintenant );

   private:

      Activite matin;
      Activite soir;
      Etat     etat;

   friend struct VanneCodec;
   };
}
