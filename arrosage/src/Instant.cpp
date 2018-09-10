#include "Instant.h"

#include <ctype.h>
#include <ESP8266WiFi.h>

namespace hpms {

   struct InstantCodec : public json::CoDec {

      static const InstantCodec codec;

      InstantCodec() :
         json::CoDec(
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

Instant Instant::operator + ( uint8_t duree ) const {
   uint8_t m = minute + duree;
   uint8_t h = heure + m / 60;
   m %= 60;
   h %= 24;
   if( h < heure ) {
      h = 23;
      m = 59;
   }
   return Instant( h, m );
}
