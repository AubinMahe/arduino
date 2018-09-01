#include "Vanne.h"

using namespace hpms;

void Vanne::ouvrir() {
   ouverte = true;
   digitalWrite( pin, HIGH );
   Serial.print( "Ouverture de la vanne n°" );
   Serial.println( pin );
}

void Vanne::fermer() {
   ouverte = false;
   digitalWrite( pin, LOW );
   Serial.print( "Fermeture de la vanne n°" );
   Serial.println( pin );
}

void Vanne::evaluer( const Instant & maintenant ) {
   if( ouverte ) {
      if( matin.est( maintenant ) || soir.est( maintenant )) {
         fermer();
      }
   }
   else {
      if( matin.est( maintenant ) || soir.est( maintenant )) {
         ouvrir();
      }
   }
}

/**
 * Analyse une chaine de catactères pour en extraire les informations
 * temporelles d'ouverture et de fermeture de vanne pour matin et soir.
 * @param hhsmm une chaine respectant le format hh:mm-hh:mm|hh:mm-hh:mm
 * @return vrai si la chaine respecte le format et que les informations
 * temporelles sont valides.
 */
bool Vanne::decode( char activite[] ) {
   return
      ( activite[3*4-1] == '|' )
      && matin.decode( activite )
      && soir .decode( activite );
}
