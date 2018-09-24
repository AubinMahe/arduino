#pragma once

#include "Arrosage.h"

namespace hpms {

   template<class A>
   struct Commande : public json::IJSonData {

      Commande() {
         /**/
      }

      char commande[30];
      A    argument;

      struct CoDec : public json::CoDec {
         CoDec() :
            json::CoDec( "Commande<?>",
               new json::String( "commande", &Commande::commande,
               new json::Object( "argument", &Commande::argument )))
         {}
      } codec;

      virtual const json::CoDec & getCoDec() const { return codec; }
   };

   struct EtatDesVannes : public json::IJSonData {

      EtatDesVannes( const Arrosage & arrosage );

      virtual const json::CoDec & getCoDec() const;

      bool etat_des_vannes[Arrosage::NBR_VANNES];
   };

   struct Configuration : public json::IJSonData {

      Configuration( const Arrosage & a = Arrosage()) :
         arrosage( a )
      {}

      virtual const json::CoDec & getCoDec() const;

      Arrosage arrosage;
   };

   struct DemarrerArreter : public json::IJSonData {

      DemarrerArreter() :
         demarrer( false )
      {}

      virtual const json::CoDec & getCoDec() const;

      bool demarrer;
   };

   struct CommanderUneVanne : public json::IJSonData {

      CommanderUneVanne() :
         pin ( -1 ),
         etat( Vanne::CONFIGUREE_FERMEE )
      {}

      virtual const json::CoDec & getCoDec() const;

      int         pin;
      Vanne::Etat etat;
   };

   struct CommanderLesVannes : public json::IJSonData {

      virtual const json::CoDec & getCoDec() const;

      CommanderUneVanne les_vannes[100];
   };
}
