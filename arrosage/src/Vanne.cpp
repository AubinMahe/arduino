#include "Vanne.h"

#include <ESP8266WiFi.h>

namespace hpms {

   struct VanneCodec : public json::CoDec {

      static const VanneCodec codec;

      VanneCodec() :
         json::CoDec( "Vanne",
            new json::Object ( "matin"  , &Vanne::matin,
            new json::Object ( "soir"   , &Vanne::soir,
            new json::Boolean( "ouverte", &Vanne::ouverte ))))
      {}
   };
}

using namespace hpms;

const VanneCodec VanneCodec::codec;

Vanne::Vanne( void ) :
   ouverte( false )
{}

Vanne::Vanne( const Activite & m, const Activite & s ) :
   matin( m ),
   soir( s ),
   ouverte( false )
{}

const json::CoDec & Vanne::getCoDec( void ) const {
   return VanneCodec::codec;
}

void Vanne::ouvrir( uint8_t pin ) {
   ouverte = true;
   digitalWrite( pin, HIGH );
   Serial.print( "Ouverture de la vanne n°" );
   Serial.println( pin );
}

void Vanne::fermer( uint8_t pin ) {
   ouverte = false;
   digitalWrite( pin, LOW );
   Serial.print( "Fermeture de la vanne n°" );
   Serial.println( pin );
}

void Vanne::evaluer( uint8_t pin, const Instant & maintenant ) {
   if( ouverte ) {
      if(  ( ! matin.est_activable( maintenant ))
         &&( ! soir .est_activable( maintenant )))
      {
         fermer( pin );
      }
   }
   else {
      if(   matin.est_activable( maintenant )
         || soir .est_activable( maintenant ))
      {
         ouvrir( pin );
      }
   }
}
