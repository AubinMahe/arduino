#pragma once

#include "json/Generator.h"
#include "json/Parser.h"

namespace hpms {

   class Instant {
   public:

      Instant( int h = 0, int m = 0 );

   public:

      bool operator > ( const Instant & r ) const;

      bool operator < ( const Instant & r ) const;

   public:

      json::Status decode( const char * name, json::Parser & parser );

      json::Status encode( json::Generator & generator ) const;

   protected:

      unsigned char heure;  // 0..23
      unsigned char minute; // 0..59
   };
}
