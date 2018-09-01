#pragma once

#include "Instant.h"

namespace hpms {

   class Activite {
   public:

      Activite( void ) {}

      Activite( const Instant & o, const Instant & f ) :
         ouverture( o ),
         fermeture( f )
      {}

      bool est( const Instant & maintenant ) const;

      /**
       * Analyse une chaine de catactères pour en extraire les informations
       * temporelles d'ouverture et de fermeture de vanne.
       * @param hhsmm une chaine respectant le format hh:mm-hh:mm
       * @return vrai si la chaine respecte le format et que les informations
       * temporelles sont valides.
       */
      bool decode( char activite[] );

   private:

      Instant ouverture;
      Instant fermeture;
   };
}
