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

json::Status Instant::decode( const char * name, json::Parser & parser ) {
   if( 0 == strcmp( name, "heure" )) {
      return parser.get( heure );
   }
   if( 0 == strcmp( name, "minute" )) {
      return parser.get( minute );
   }
   return json::UNEXPECTED_ATTRIBUTE;
}

json::Status Instant::encode( json::Generator & generator ) const {
   json::Status status = json::SUCCESS;
   if( status == json::SUCCESS ) {
      status = generator.encode( "heure", heure );
   }
   if( status == json::SUCCESS ) {
      status = generator.encode( "minute", minute );
   }
   return status;
}
