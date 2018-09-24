#include "Journal.h"

#include <string.h>

namespace hpms {

   struct Evenement {

      Evenement( const Instant & i, uint8_t p, bool e, Evenement * s = 0 ) :
         instant( i ),
         pin( p ),
         etat( e ),
         suivant( s )
      {}

      Instant     instant;
      uint8_t     pin;
      bool        etat;
      Evenement * suivant;
   };
}

using namespace hpms;

Journal * Journal::le_journal = 0;

Journal::Journal( Horloge & h ) :
   horloge( h ),
   evenements( 0 )
{
   le_journal = this;
}

bool Journal::encode( char * json, size_t size, bool autotest ) const {
   json[0] = '\0';
   ::snprintf( json, size, "{\"autotest\":%s,\"heure\":%d,\"minute\":%d,\"events\":[",
      autotest ? "true" : "false", horloge.get_heure(), horloge.get_minute());
   for( Evenement * ev = evenements; ev; ev = ev->suivant ) {
      char tmp[50];
      ::snprintf( tmp, sizeof( tmp ), "{\"heure\":%d,\"minute\":%d,\"pin\":%d,\"etat\":%s}%s",
         ev->instant.get_heure(), ev->instant.get_minute(),
         ev->pin,
         ev->etat ? "true" : "false",
         ev->suivant ? "," : "" );
      strncat( json, tmp, size - strlen( json ));
   }
   size_t len = strlen( json );
   strncat( json, "]}", size - len );
   return len < size;
}

void Journal::arrosage( bool etat ) {
   evenements = new Evenement( horloge, 0xFF, etat, evenements );
}

void Journal::vanne( uint8_t pin, bool etat ) {
   evenements = new Evenement( horloge, pin, etat, evenements );
}

void Journal::efface( void ) {
   delete evenements;
   evenements = 0;
}
