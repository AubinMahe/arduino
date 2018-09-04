#pragma once

#include <Arduino.h>

namespace hpms {

   struct Mesure {
      float             seuil;
      unsigned long int occurences;
   };

   class RecueilDeTemperatures {
   public:

      RecueilDeTemperatures( uint8_t pin, size_t plages, ... );

      void mesure( void );

      const Mesure * getMesures( size_t & nombre ) const;

   private:

      uint8_t  _pin;
      size_t   _plages;
      Mesure * _mesures;

   private:
      RecueilDeTemperatures( const RecueilDeTemperatures & );
      RecueilDeTemperatures & operator = ( const RecueilDeTemperatures & );
   };
}
