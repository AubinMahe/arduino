#pragma once

#include "Horloge.h"
#include "Vanne.h"

namespace hpms {

   class Arrosage : public json::IJSonData {
   public:

      static const uint8_t NBR_VANNES = 4;

      Arrosage();

   public:

      const json::CoDec & getCoDec( void ) const;

   public:

      void actualiser( void );

      void actualiser( uint8_t heure, uint8_t minute );

      void a_ete_actualise( void );

   public:

      void demarrage_de_l_auto_test( void );

      bool est_en_auto_test( void ) const;

      void fin_de_l_auto_test( void );

      void demarrer( bool demarrer );

      void commander_une_vanne( uint8_t pin, Vanne::Etat etat );

      void evaluer( void );

   private:

      bool    auto_test_en_cours;
      bool    est_en_marche;
      Horloge horloge;
      Vanne   vannes[NBR_VANNES];

   friend struct ArrosageCoDec;
   };
}
