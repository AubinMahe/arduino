#pragma once

#include "json/Encoder.h"
#include "json/Decoder.h"

namespace hpms {

   class Instant : public json::IJSonData {
   public:

      Instant( int h = 0, int m = 0 );

   public:

      virtual const json::CoDec & getCoDec() const;

   public:

      bool operator > ( const Instant & r ) const;

      bool operator < ( const Instant & r ) const;

      Instant operator + ( uint8_t duree ) const;

   protected:

      unsigned char heure;  // 0..23
      unsigned char minute; // 0..59

   friend struct InstantCodec;
   };
}
