#pragma once

namespace hpms {

   class Instant {
   public:

      Instant( int h = 0, int m = 0 ) :
         heure ( h ),
         minute( m )
      {}

      /**
       * Analyse une chaine de catactères pour en extraire les informations
       * d'heures et de minutes.
       * @param hhsmm une chaine respectant le format hh:mm
       * @return vrai si la chaine respecte le format et que 0 <= heure < 24
       * et que 0 <= minute < 59.
       */
      bool decode( char hhsmm[] );

      bool operator > ( const Instant & r ) const;

      bool operator < ( const Instant & r ) const;

   protected:

      unsigned char heure;  // 0..23
      unsigned char minute; // 0..59
   };
}
