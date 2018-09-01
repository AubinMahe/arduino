#pragma once

#include "Instant.h"

namespace hpms {

   /**
    * Cette horloge utilise NTP pour obtenir l'heure absolue une fois par 24 heures.
    * Entre temps, elle utilise l'horloge temps-réel du processeur 8266.
    */
   class Horloge : public Instant {
   public:

      Horloge() :
         derniere_mise_a_l_heure( 0 ),
         refHeure ( 0 ),
         refMinute( 0 )
      {}

      void actualiser();

   private:

      static const unsigned long UNE_JOURNEE = 24*60*60*1000; // en ms

      unsigned long derniere_mise_a_l_heure;
      unsigned char refHeure;  // 0..23 la dernière heure  NTP
      unsigned char refMinute; // 0..59 la dernière minute NTP
   };
}
