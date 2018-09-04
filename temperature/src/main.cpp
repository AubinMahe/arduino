#include "RecueilDeTemperatures.h"

static hpms::RecueilDeTemperatures recueil( A0, 7, 20, 25, 50, 45, 40, 35, 30 );
static unsigned long int           periode;
static unsigned long int           instant;

void setup( void ) {
   Serial.begin( 115200 );
   periode = 10UL*1000UL;
   instant = periode;
}

void loop( void ) {
   delay( 1000 );
   recueil.mesure();
   if( millis() > instant ) {
      instant += periode;
      size_t plages;
      const hpms::Mesure * mesures = recueil.getMesures( plages );
      Serial.println( "Nombre de relevés supérieur à :" );
      for( size_t i = 0; i < plages; ++i ) {
         Serial.print( "   " );
         Serial.print( mesures[i].seuil );
         Serial.print( "° : " );
         Serial.println( mesures[i].occurences );
      }
   }
}
