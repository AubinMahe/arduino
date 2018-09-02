#pragma once

#include "Instant.h"

namespace hpms {

   class Activite {
   public:

      Activite( const Instant & o = Instant(), const Instant & f = Instant());

   public:

      bool est( const Instant & maintenant ) const;

   public:

      json::Status decode( const char * name, json::Parser & parser );

      json::Status encode( json::Generator & generator ) const;

   private:

      Instant ouverture;
      Instant fermeture;
   };
}
