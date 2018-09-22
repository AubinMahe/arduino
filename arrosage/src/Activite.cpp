#include "Activite.h"

#include <ESP8266WiFi.h>

namespace hpms {

   struct ActiviteCodec : public json::CoDec {

      static ActiviteCodec codec;

      ActiviteCodec() :
         json::CoDec( "Activite",
            new json::Object( "ouverture", &Activite::ouverture,
            new json::Byte  ( "duree"    , &Activite::duree_minutes )))
      {}
   };
}

using namespace hpms;

ActiviteCodec ActiviteCodec::codec;

Activite::Activite( const Instant & o, uint8_t d ) :
   ouverture( o ),
   duree_minutes( d & 0x3F ) // 1 heure maximum soit 6 bits 2^6-1 = 0..63
{}

bool Activite::est_activable( const Instant & maintenant ) const {
   bool after  = ( ouverture < maintenant );
   bool before = ( maintenant < ( ouverture + duree_minutes ));
   return after && before;
}

const json::CoDec & Activite::getCoDec() const {
   return ActiviteCodec::codec;
}
