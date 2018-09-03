#pragma once

#include "Status.h"

namespace json {

   class Decoder;
   class Encoder;

   struct IJSonData {
      virtual ~ IJSonData() {}
      virtual Status decode( const char * name, Decoder & decoder ) = 0;
      virtual Status encode( Encoder & encoder ) const = 0;
   };
}
