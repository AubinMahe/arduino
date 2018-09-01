#include "Instant.h"

#include <ctype.h>

using namespace hpms;

bool Instant::decode( char hhsmm[] ) {
   if(   isdigit( hhsmm[0] )
      && isdigit( hhsmm[1] )
      &&(         hhsmm[2] == ':' )
      && isdigit( hhsmm[3] )
      && isdigit( hhsmm[4] ))
   {
      int h = ( hhsmm[0] - '0' ) * 10 + ( hhsmm[1] - '0' );
      int m = ( hhsmm[2] - '0' ) * 10 + ( hhsmm[3] - '0' );
      if(( -1 < m )&&( m < 60 )&&( -1 < h )&&( h < 24 )) {
         heure  = h;
         minute = m;
         return true;
      }
   }
   return false;
}

bool Instant::operator > ( const Instant & r ) const {
   if( heure > r.heure ) {
      return true;
   }
   if( heure < r.heure ) {
      return false;
   }
   return minute > r.minute;
}

bool Instant::operator < ( const Instant & r ) const {
   if( heure < r.heure ) {
      return true;
   }
   if( heure < r.heure ) {
      return false;
   }
   return minute < r.minute;
}
