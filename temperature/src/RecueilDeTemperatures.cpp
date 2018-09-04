#include "RecueilDeTemperatures.h"

#include <stdarg.h>
#include <stdlib.h>

using namespace hpms;

RecueilDeTemperatures::RecueilDeTemperatures( uint8_t pin, size_t plages, ... ) :
   _pin( pin ),
   _plages( plages ),
   _mesures( new Mesure[plages] )
{
   va_list args;
   va_start( args, plages );
   for( size_t i = 0; i < _plages; ++i ) {
      _mesures[i].seuil      = va_arg( args, int );
      _mesures[i].occurences = 0UL;
   }
   qsort( _mesures, _plages, sizeof( Mesure ),
      []( const void * l, const void * r ) {
      const Mesure * left  = (const Mesure *)l;
      const Mesure * right = (const Mesure *)r;
      return (int)(-( left->seuil - right->seuil ));
   });
}

void RecueilDeTemperatures::mesure( void ) {
   float voltage = ( analogRead( _pin ) / 1024.0f ) * 5.0f;
   float celcius = ( voltage - 0.5f ) * 100.0f;
   for( size_t i = 0; i < _plages; ++i ) {
      if( celcius > _mesures[i].seuil ) {
         ++_mesures[i].occurences;
         break;
      }
   }
}

const Mesure * RecueilDeTemperatures::getMesures( size_t & plages ) const {
   plages = _plages;
   return _mesures;
}
