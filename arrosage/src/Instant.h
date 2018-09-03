#pragma once

#include "json/Encoder.h"
#include "json/Decoder.h"

namespace hpms {

   class Instant : private json::IJSonData {
   public:

      Instant( int h = 0, int m = 0 );

   public:

      bool operator > ( const Instant & r ) const;

      bool operator < ( const Instant & r ) const;

   public:

      json::Status decode( const char * name, json::Decoder & decoder );

      json::Status encode( json::Encoder & encoder ) const;

   protected:

      unsigned char heure;  // 0..23
      unsigned char minute; // 0..59
   };
}
