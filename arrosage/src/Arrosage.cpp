#include "Arrosage.h"
#include "Log.h"

namespace hpms {

   struct ArrosageCoDec : public json::CoDec {

      static ArrosageCoDec codec;

      ArrosageCoDec() :
         json::CoDec( "Arrosage",
            new json::Boolean    ( "est_en_marche", &Arrosage::est_en_marche,
            new json::Object     ( "horloge"      , &Arrosage::horloge,
            new json::ObjectArray( "vannes"       , &Arrosage::vannes   ))))
      {}
   };
}

using namespace hpms;

ArrosageCoDec ArrosageCoDec::codec;

const uint8_t Arrosage::NBR_VANNES;

Arrosage::Arrosage() :
   auto_test_en_cours( false ),
   est_en_marche( false ),
   vannes{{{{ 8,  0 }, 20 }, {{ 22,  0 }, 20 }},
          {{{ 8, 15 }, 20 }, {{ 22, 15 }, 20 }},
          {{{ 8, 30 }, 20 }, {{ 22, 30 }, 20 }},
          {{{ 8, 45 }, 20 }, {{ 22, 45 }, 20 }} }
{}

const json::CoDec & Arrosage::getCoDec( void ) const {
   return ArrosageCoDec::codec;
}

void Arrosage::actualiser( void ) {
   if( ! auto_test_en_cours ) {
      horloge.actualiser( false );
   }
}

void Arrosage::actualiser( uint8_t heure, uint8_t minute ) {
   if( ! auto_test_en_cours ) {
      horloge.actualiser( heure, minute );
   }
}

void Arrosage::a_ete_actualise( void ) {
   if( ! auto_test_en_cours ) {
      horloge.actualiser( horloge.get_heure(), horloge.get_minute());
   }
}

void Arrosage::demarrage_de_l_auto_test( void ) {
   Log( "Arrosage::demarrage_de_l_auto_test" );
   auto_test_en_cours = true;
}

bool Arrosage::est_en_auto_test( void ) const {
   return auto_test_en_cours;
}

void Arrosage::demarrer( bool demarrer ) {
   est_en_marche = demarrer;
}

void Arrosage::commander_une_vanne( uint8_t pin, Vanne::Etat etat ) {
   Log( "Arrosage::commander_une_vanne( %d, %d )", pin, etat );
   if( ! auto_test_en_cours && ( pin < countof( vannes ))) {
      vannes[pin].forcer_l_etat( pin, horloge, etat );
   }
}

void Arrosage::evaluer() {
   if( horloge.actualiser( auto_test_en_cours )) {
      Log( "Arrosage::evaluer|fin de l'autotest." );
      auto_test_en_cours = false;
   }
   if( est_en_marche || auto_test_en_cours ) {
      for( size_t pin = 0; pin < countof( vannes ); ++pin ) {
         vannes[pin].evaluer( pin, horloge );
      }
   }
}
