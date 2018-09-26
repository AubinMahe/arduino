#pragma once

#include "Horloge.h"

namespace hpms {

   struct Evenement;
   class Journal {
   public:

      static Journal * le_journal;

      Journal( Horloge & horloge );

   public:

      bool encode( char * json, size_t size, bool autotest ) const;

   public:

      void arrosage( bool etat );
      void vanne( uint8_t pin, bool etat );
      void effacer( void );

   private:

      Horloge &   horloge;
      Evenement * evenements;
   };
}
