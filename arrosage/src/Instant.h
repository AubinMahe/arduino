#pragma once

#include "json/Encoder.h"
#include "json/Decoder.h"

#include <Arduino.h>

namespace hpms {

   class Instant : public json::IJSonData {
   public:

      Instant( int h = 0, int m = 0 );

   public:

      virtual const json::CoDec & getCoDec() const;

   public:

      bool operator > ( const Instant & r ) const;

      bool operator < ( const Instant & r ) const;

      Instant operator + ( uint8_t duree_minutes ) const;

      uint8_t get_heure() const { return heure; }

      uint8_t get_minute() const { return minute; }

   protected:

      uint8_t heure;  // 0..23
      uint8_t minute; // 0..59

   friend struct InstantCodec;
   };
}
