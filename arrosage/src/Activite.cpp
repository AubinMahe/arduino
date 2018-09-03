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

json::Status Activite::decode( const char * name, json::Decoder & decoder ) {
   if( 0 == strcmp( name, "ouverture" )) {
      return decoder.decode( ouverture );
   }
   if( 0 == strcmp( name, "fermeture" )) {
      return decoder.decode( fermeture );
   }
   return json::UNEXPECTED_ATTRIBUTE;
}

json::Status Activite::encode( json::Encoder & encoder ) const {
   json::Status status = json::SUCCESS;
   if( status == json::SUCCESS ) {
      status = encoder.encodeObject( "ouverture", ouverture );
   }
   if( status == json::SUCCESS ) {
      status = encoder.encodeObject( "fermeture", fermeture );
   }
   return status;
}
