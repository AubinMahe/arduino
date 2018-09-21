#include "Vanne.h"

#include <ESP8266WiFi.h>

namespace hpms {

   struct VanneCodec : public json::CoDec {

      static const VanneCodec codec;

      VanneCodec() :
         json::CoDec(
            new json::Byte   ( "pin"    , &Vanne::pin,
            new json::Object ( "matin"  , &Vanne::matin,
            new json::Object ( "soir"   , &Vanne::soir,
            new json::Boolean( "ouverte", &Vanne::ouverte )))))
      {}
   };
}

using namespace hpms;

const VanneCodec VanneCodec::codec;

bool Vanne::pin_est_valide( uint8_t pin ) {
   return pin > 0 && pin <= MAX_PIN;
}

Vanne::Vanne( void ) :
   pin( 0 ),
   ouverte( false )
{}

Vanne::Vanne( uint8_t p, const Activite & m, const Activite & s ) :
   pin( p ),
   matin( m ),
   soir( s ),
   ouverte( false )
{}

const json::CoDec & Vanne::getCoDec( void ) const {
   return VanneCodec::codec;
}

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
      if(  ( ! matin.est_activable( maintenant ))
         &&( ! soir .est_activable( maintenant )))
      {
         fermer();
      }
   }
   else {
      if(   matin.est_activable( maintenant )
         || soir .est_activable( maintenant ))
      {
         ouvrir();
      }
   }
}
