#include "Activite.h"

using namespace hpms;

bool Activite::est( const Instant & maintenant ) const {
   return( maintenant > ouverture )&&( maintenant < fermeture );
}

/**
 * Analyse une chaine de catactères pour en extraire les informations
 * temporelles d'ouverture et de fermeture de vanne.
 * @param hhsmm une chaine respectant le format hh:mm-hh:mm
 * @return vrai si la chaine respecte le format et que les informations
 * temporelles sont valides.
 */
bool Activite::decode( char activite[] ) {
   return( activite[5] == '-' ) && ouverture.decode( activite ) && fermeture.decode( activite );
}
