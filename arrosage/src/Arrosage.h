#pragma once

#include "Horloge.h"
#include "Vanne.h"
#include "Journal.h"

namespace hpms {

   class Arrosage : public json::IJSonData {
   public:

      static const uint8_t NBR_VANNES = 4;

      Arrosage( void );

   public:

      const json::CoDec & getCoDec( void ) const;

   public:

      void actualiser( void );

      void actualiser( uint8_t heure, uint8_t minute );

      void a_ete_actualise( void );

   public:

      void demarrer( bool demarrer );

      void demarrage_de_l_auto_test( void );
      void arret_de_l_auto_test( void );
      bool est_en_auto_test( void ) const;

      bool vanne_est_ouverte( size_t pin ) const;

      void commander_une_vanne( uint8_t pin, Vanne::Etat etat );

      void evaluer( void );

   private:

      bool      auto_test_en_cours;
      bool      est_en_marche;
      Horloge   horloge;
      Vanne     vannes[NBR_VANNES];

   friend struct ArrosageCoDec;
   };
}
