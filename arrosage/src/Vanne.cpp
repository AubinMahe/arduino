#include "Vanne.h"
#include "Log.h"
#include "Journal.h"

#include <ESP8266WiFi.h>

namespace hpms {

   struct VanneCodec : public json::CoDec {

      static const VanneCodec codec;

      VanneCodec() :
         json::CoDec( "Vanne",
            new json::Object( "matin", &Vanne::matin,
            new json::Object( "soir" , &Vanne::soir,
            new json::Enum8 ( "etat" , &Vanne::etat ))))
      {}
   };
}

using namespace hpms;

const VanneCodec VanneCodec::codec;

Vanne::Vanne( void ) :
   etat( CONFIGUREE_FERMEE )
{}

Vanne::Vanne( const Activite & m, const Activite & s ) :
   matin( m ),
   soir( s ),
   etat( CONFIGUREE_FERMEE )
{}

const json::CoDec & Vanne::getCoDec( void ) const {
   return VanneCodec::codec;
}

bool Vanne::est_ouverte( uint8_t pin ) const {
   return digitalRead( pin );
}

void Vanne::forcer_l_etat( uint8_t pin, const Instant & maintenant, Vanne::Etat nouvel_etat ) {
   Log( "Vanne::forcer_l_etat( %d, %0d:%0d, %d )",
      pin, maintenant.get_heure(), maintenant.get_minute(), nouvel_etat );
   if( nouvel_etat == FORCEE_FERMEE ) {
      etat = FORCEE_FERMEE;
      fermer( pin, maintenant );
   }
   else if( nouvel_etat == FORCEE_OUVERTE ) {
      etat = FORCEE_OUVERTE;
      ouvrir( pin, maintenant );
   }
   else if(( nouvel_etat == CONFIGUREE_OUVERTE )||( nouvel_etat == CONFIGUREE_FERMEE )) {
      int r = digitalRead( pin );
      if( r == LOW ) {
         etat = CONFIGUREE_FERMEE;
      }
      else {
         etat = CONFIGUREE_OUVERTE;
      }
      evaluer( pin, maintenant );
   }
}

void Vanne::evaluer( uint8_t pin, const Instant & maintenant ) {
   if(( etat == CONFIGUREE_OUVERTE )||( etat == CONFIGUREE_FERMEE )) {
      if(   matin.doit_etre_ouverte( maintenant )
         || soir .doit_etre_ouverte( maintenant ))
      {
         if( etat == CONFIGUREE_FERMEE ) {
            etat = CONFIGUREE_OUVERTE;
            ouvrir( pin, maintenant );
         }
      }
      else {
         if( etat == CONFIGUREE_OUVERTE ) {
            etat = CONFIGUREE_FERMEE;
            fermer( pin, maintenant );
         }
      }
   }
}

void Vanne::ouvrir( uint8_t pin, const Instant & maintenant ) {
   digitalWrite( pin, HIGH );
   Journal::le_journal->vanne( pin, true );
   Log( "Vanne::ouvrir( %d, %0d:%0d )", pin, maintenant.get_heure(), maintenant.get_minute());
}

void Vanne::fermer( uint8_t pin, const Instant & maintenant ) {
   digitalWrite( pin, LOW );
   Journal::le_journal->vanne( pin, false );
   Log( "Vanne::fermer( %d, %0d:%0d )", pin, maintenant.get_heure(), maintenant.get_minute());
}
