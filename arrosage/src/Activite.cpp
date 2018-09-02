#include "Activite.h"

#include <ESP8266WiFi.h>

using namespace hpms;

Activite::Activite( const Instant & o, const Instant & f ) :
   ouverture( o ),
   fermeture( f )
{}

bool Activite::est( const Instant & maintenant ) const {
   return( maintenant > ouverture )&&( maintenant < fermeture );
}

json::Status Activite::decode( const char * name, json::Parser & parser ) {
   if( 0 == strcmp( name, "ouverture" )) {
      return parser.decode( ouverture );
   }
   if( 0 == strcmp( name, "fermeture" )) {
      return parser.decode( fermeture );
   }
   return json::UNEXPECTED_ATTRIBUTE;
}

json::Status Activite::encode( json::Generator & generator ) const {
   json::Status status = json::SUCCESS;
   if( status == json::SUCCESS ) {
      status = generator.encodeObject( "ouverture", ouverture );
   }
   if( status == json::SUCCESS ) {
      status = generator.encodeObject( "fermeture", fermeture );
   }
   return status;
}
