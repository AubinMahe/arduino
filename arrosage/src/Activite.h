#pragma once

#include "Instant.h"

namespace hpms {

   class Activite : private json::IJSonData {
   public:

      Activite( const Instant & o = Instant(), const Instant & f = Instant());

   public:

      bool est( const Instant & maintenant ) const;

   public:

      json::Status decode( const char * name, json::Decoder & decoder );

      json::Status encode( json::Encoder & encoder ) const;

   private:

      Instant ouverture;
      Instant fermeture;
   };
}
