#pragma once

#include "Instant.h"

namespace hpms {

   /**
    * Cette horloge utilise NTP pour obtenir l'heure absolue une fois par 24 heures.
    * Entre temps, elle utilise l'horloge temps-réel du processeur 8266.
    */
   class Horloge : public Instant {
   public:

      Horloge();

      void actualiser( void );

      void actualiser( uint8_t heure, uint8_t minute );

   private:

      unsigned long derniere_mise_a_l_heure;
      unsigned char refHeure;  // 0..23 la dernière heure  NTP
      unsigned char refMinute; // 0..59 la dernière minute NTP
   };
}
