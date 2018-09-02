#include "Vanne.h"

#include <ESP8266WiFi.h>

using namespace hpms;

bool Vanne::pin_est_valide( uint8_t pin ) {
   return pin > 0 && pin <= MAX_PIN;
}

Vanne::Vanne( void ) :
   pin( 0 ),
   ouverte( false )
{}

Vanne::Vanne( uint8_t p, const Activite & m, const Activite & s ) :
   pin( p ),
   ouverte( false ),
   matin( m ),
   soir( s )
{}

bool Vanne::est( uint8_t p ) const {
   return pin == p;
}

void Vanne::ouvrir() {
   ouverte = true;
   digitalWrite( pin, HIGH );
   Serial.print( "Ouverture de la vanne n°" );
   Serial.println( pin );
}

void Vanne::fermer() {
   ouverte = false;
   digitalWrite( pin, LOW );
   Serial.print( "Fermeture de la vanne n°" );
   Serial.println( pin );
}

void Vanne::evaluer( const Instant & maintenant ) {
   if( ouverte ) {
      if( matin.est( maintenant ) || soir.est( maintenant )) {
         fermer();
      }
   }
   else {
      if( matin.est( maintenant ) || soir.est( maintenant )) {
         ouvrir();
      }
   }
}

json::Status Vanne::decode( const char * name, json::Parser & parser ) {
   if( 0 == strcmp( name, "pin" )) {
      return parser.get( pin );
   }
   if( 0 == strcmp( name, "ouverte" )) {
      return parser.get( ouverte );
   }
   if( 0 == strcmp( name, "matin" )) {
      return parser.decode( matin );
   }
   if( 0 == strcmp( name, "soir" )) {
      return parser.decode( soir );
   }
   return json::UNEXPECTED_ATTRIBUTE;
}

json::Status Vanne::encode( json::Generator & generator ) const {
   json::Status status = json::SUCCESS;
   if( status == json::SUCCESS ) {
      status = generator.encode( "pin", pin );
   }
   if( status == json::SUCCESS ) {
      status = generator.encode( "ouverte", ouverte );
   }
   if( status == json::SUCCESS ) {
      status = generator.encodeObject( "matin", matin );
   }
   if( status == json::SUCCESS ) {
      status = generator.encodeObject( "soir", soir );
   }
   return status;
}
