#include "Arrosage.h"

namespace hpms {

   struct ArrosageCoDec : public json::CoDec {

      static ArrosageCoDec codec;

      ArrosageCoDec() :
         json::CoDec(
            new json::Boolean    ( "est_en_marche", &Arrosage::est_en_marche,
            new json::ObjectArray( "vannes"       , &Arrosage::vannes )))
      {}
   };
}

using namespace hpms;

ArrosageCoDec ArrosageCoDec::codec;

Arrosage::Arrosage() :
   est_en_marche( false ),
   vannes{{ 1, {{ 8,  0 }, 20 }, {{ 22,  0 }, 20 }},
          { 2, {{ 8, 15 }, 20 }, {{ 22, 15 }, 20 }},
          { 3, {{ 8, 30 }, 20 }, {{ 22, 30 }, 20 }},
          { 4, {{ 8, 45 }, 20 }, {{ 22, 45 }, 20 }} }
{}

const json::CoDec & Arrosage::getCoDec( void ) const {
   return ArrosageCoDec::codec;
}

void Arrosage::demarrer( bool demarrer ) {
   est_en_marche = demarrer;
}

void Arrosage::commander_une_vanne( uint8_t pin, bool ouvrir ) {
   for( size_t i = 0; i < sizeof( vannes ) / sizeof( vannes[0] ); ++i ) {
      if( vannes[i].est( pin )) {
         if( ouvrir ) {
            vannes[i].ouvrir();
         }
         else {
            vannes[i].fermer();
         }
      }
   }
}

void Arrosage::evaluer( Horloge & horloge ) {
   if( est_en_marche ) {
      for( size_t i = 0; i < sizeof( vannes ) / sizeof( vannes[0] ); ++i ) {
         vannes[i].evaluer( horloge );
      }
   }
}
