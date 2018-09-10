#include "Activite.h"

#include <ESP8266WiFi.h>

namespace hpms {

   struct ActiviteCodec : public json::CoDec {

      static ActiviteCodec codec;

      ActiviteCodec() :
         json::CoDec(
            new json::Object( "ouverture", &Activite::ouverture,
            new json::Byte  ( "duree"    , &Activite::duree )))
      {}
   };
}

using namespace hpms;

ActiviteCodec ActiviteCodec::codec;

Activite::Activite( const Instant & o, uint8_t d ) :
   ouverture( o ),
   duree( d & 0x3F ) // 1 heure maximum soit 6 bits 2^6 = 64
{}

bool Activite::est( const Instant & maintenant ) const {
   return( maintenant > ouverture )&&( maintenant < ouverture + duree );
}

const json::CoDec & Activite::getCoDec() const {
   return ActiviteCodec::codec;
}
