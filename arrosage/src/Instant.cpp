#include "Instant.h"

#include <ctype.h>
#include <ESP8266WiFi.h>

using namespace hpms;

Instant::Instant( int h, int m ) :
   heure ( h ),
   minute( m )
{}

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

json::Status Instant::decode( const char * name, json::Decoder & decoder ) {
   if( 0 == strcmp( name, "heure" )) {
      return decoder.get( heure );
   }
   if( 0 == strcmp( name, "minute" )) {
      return decoder.get( minute );
   }
   return json::UNEXPECTED_ATTRIBUTE;
}

json::Status Instant::encode( json::Encoder & encoder ) const {
   json::Status status = json::SUCCESS;
   if( status == json::SUCCESS ) {
      status = encoder.encode( "heure", heure );
   }
   if( status == json::SUCCESS ) {
      status = encoder.encode( "minute", minute );
   }
   return status;
}
