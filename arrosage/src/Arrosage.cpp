#include "Arrosage.h"

using namespace hpms;

Arrosage::Arrosage() :
   est_en_marche( false ),
   vannes{{ 1, {{ 8,  0 },{ 8, 15 }}, {{ 22,  0 },{ 22, 15 }}},
          { 2, {{ 8, 15 },{ 8, 30 }}, {{ 22, 15 },{ 22, 30 }}},
          { 3, {{ 8, 30 },{ 8, 45 }}, {{ 22, 30 },{ 22, 45 }}},
          { 4, {{ 8, 45 },{ 9,  0 }}, {{ 22, 45 },{ 23,  0 }}} }
{}

void Arrosage::demarrer( bool demarrer ) {
   est_en_marche = demarrer;
}

void Arrosage::commander_une_vanne( uint8_t pin, bool ouvrir ) {
   for( size_t i = 0; i < sizeof( vannes ) / sizeof( vannes[0] ); ++i ) {
      if( vannes[i].est( pin )) {
         if( ouvrir ) {
            vannes[i].ouvrir();
         }
         else {
            vannes[i].fermer();
         }
      }
   }
}

void Arrosage::evaluer( Horloge & horloge ) {
   if( est_en_marche ) {
      for( size_t i = 0; i < sizeof( vannes ) / sizeof( vannes[0] ); ++i ) {
         vannes[i].evaluer( horloge );
      }
   }
}

json::Status Arrosage::decode( const char * name, json::Decoder & decoder ) {
   if( 0 == strcmp( name, "est_en_marche" )) {
      return decoder.get( est_en_marche );
   }
   if( 0 == strcmp( name, "vannes" )) {
      return decoder.decode( vannes, sizeof( vannes ) / sizeof( vannes[0] ));
   }
   return json::UNEXPECTED_ATTRIBUTE;
}

json::Status Arrosage::encode( json::Encoder & encoder ) const {
   json::Status status = json::SUCCESS;
   if( status == json::SUCCESS ) {
      status = encoder.encode( "est_en_marche", est_en_marche );
   }
   if( status == json::SUCCESS ) {
      status = encoder.encodeObjectArray<Vanne>(
         "vannes", vannes, sizeof( vannes ) / sizeof( vannes[0] ));
   }
   return status;
}
