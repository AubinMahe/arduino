#include "Arrosage.h"

namespace hpms {

   struct ArrosageCoDec : public json::CoDec {

      static ArrosageCoDec codec;

      ArrosageCoDec() :
         json::CoDec( "Arrosage",
            new json::Boolean    ( "est_en_marche", &Arrosage::est_en_marche,
            new json::ObjectArray( "vannes"       , &Arrosage::vannes )))
      {}
   };
}

using namespace hpms;

ArrosageCoDec ArrosageCoDec::codec;

const uint8_t Arrosage::NBR_VANNES;

Arrosage::Arrosage() :
   est_en_marche( false ),
   vannes{{{{ 8,  0 }, 20 }, {{ 22,  0 }, 20 }},
          {{{ 8, 15 }, 20 }, {{ 22, 15 }, 20 }},
          {{{ 8, 30 }, 20 }, {{ 22, 30 }, 20 }},
          {{{ 8, 45 }, 20 }, {{ 22, 45 }, 20 }} }
{}

const json::CoDec & Arrosage::getCoDec( void ) const {
   return ArrosageCoDec::codec;
}

void Arrosage::demarrer( bool demarrer ) {
   est_en_marche = demarrer;
}

void Arrosage::commander_une_vanne( uint8_t pin, bool ouvrir ) {
   if( ouvrir ) {
      vannes[pin].ouvrir( pin );
   }
   else {
      vannes[pin].fermer( pin );
   }
}

void Arrosage::evaluer( Horloge & horloge ) {
   if( est_en_marche ) {
      for( size_t pin = 0; pin < countof( vannes ); ++pin ) {
         vannes[pin].evaluer( pin, horloge );
      }
   }
}
