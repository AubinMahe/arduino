#include "Instant.h"

#include <ctype.h>
#include <ESP8266WiFi.h>

namespace hpms {

   struct InstantCodec : public json::CoDec {

      static const InstantCodec codec;

      InstantCodec() :
         json::CoDec( "Instant",
            new json::Byte( "heure" , &Instant::heure,
            new json::Byte( "minute", &Instant::minute )))
      {}
   };
}

using namespace hpms;

const InstantCodec InstantCodec::codec;

Instant::Instant( int h, int m ) :
   heure ( h ),
   minute( m )
{}

const json::CoDec & Instant::getCoDec() const {
   return InstantCodec::codec;
}

bool Instant::operator < ( const Instant & r ) const {
   if( heure < r.heure ) {
      return true;
   }
   if( heure > r.heure ) {
      return false;
   }
   return minute < r.minute;
}

bool Instant::operator <= ( const Instant & r ) const {
   if( heure < r.heure ) {
      return true;
   }
   if( heure > r.heure ) {
      return false;
   }
   return minute <= r.minute;
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

bool Instant::operator >= ( const Instant & r ) const {
   if( heure > r.heure ) {
      return true;
   }
   if( heure < r.heure ) {
      return false;
   }
   return minute >= r.minute;
}

Instant Instant::operator + ( int duree_minutes ) const {
   int m = 60*heure + minute + duree_minutes;
   int h = m / 60;
   m %= 60;
   if( h < 0 ) {
      h = 0;
      m = 0;
   }
   else if( h > 23 ) {
      h = 23;
      m = 59;
   }
   else {
      h %= 24;
   }
   return Instant( h, m );
}
