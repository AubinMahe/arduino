#pragma once

#include "Instant.h"

namespace hpms {

   class Activite : public json::IJSonData {
   public:

      Activite( const Instant & o = Instant(), uint8_t duree = 30 );

   public:

      virtual const json::CoDec & getCoDec() const;

   public:

      bool est_activable( const Instant & maintenant ) const;

   private:

      Instant ouverture;
      uint8_t duree_minutes;

   friend struct ActiviteCodec;
   };
}
