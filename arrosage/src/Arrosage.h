#pragma once

#include <ESP8266WiFi.h>

#include "Horloge.h"
#include "Vanne.h"

namespace hpms {

   class Arrosage {
   public:

      Arrosage() :
         est_en_marche( false ),
         serveur_http( HTTP_PORT ),
         tampon1{{ 1, {{ 8,  0 },{ 8, 15 }}, {{ 22,  0 },{ 22, 15 }}},
                 { 2, {{ 8, 15 },{ 8, 30 }}, {{ 22, 15 },{ 22, 30 }}},
                 { 3, {{ 8, 30 },{ 8, 45 }}, {{ 22, 30 },{ 22, 45 }}},
                 { 4, {{ 8, 45 },{ 9,  0 }}, {{ 22, 45 },{ 23,  0 }}} },
         vannes( tampon1 )
      {}

      void setup();

      void loop();

   private:

      /**
       * Le tampon de texte à envoyer par HTTP est structuré ainsi :
       *        "pp,vv:x,vv:x,vv:x,..."
       * - pp est le nombre de vanne à commander
       * - vv est le numéro de vanne
       * - x peut prendre les valeurs 'o' pour ouverte et 'f' pour fermée.
       * - La virgule termine l'expression même pour la dernière vanne.
       */
      void commander_une_vanne( WiFiClient & client );

      /**
       * Le tampon de texte à envoyer par HTTP est structuré ainsi :
       *        "pp,e,hh:mm-hh:mm|hh:mm-hh:mm,hh:mm-hh:mm|hh:mm-hh:mm,..."
       * - pp : deux caractères numériques pour spécifier le nombre de vannes
       *   à configurer.
       * - e : un caractère parmi {'m','a'} pour spécifier l'état MARCHE/ARRET
       *   du système.
       * - La configuration d'une vanne s'exprime au moyen de huit fois trois
       *   caractères pour spécifier la configuration du matin et du soir ;
       *   chacune composée de l'instant de mise en route et d'arrêt. La
       *   virgule termine l'expression même pour la dernière vanne.
       */
      void configurer( WiFiClient & client );

   private:

      static const char *        SSID;
      static const char *        WIFI_PWD;
      static const int           NOMBRE_DE_VANNES      = 4;
      static const char          CONFIGURER            = '1';
      static const char          MARCHE                = 'm';
      static const char          ARRET                 = 'a';
      static const char          COMMANDER             = '2';
      static const char          FERMER_LA_VANNE       = 'f';
      static const char          OUVRIR_LA_VANNE       = 'o';
      static const unsigned long WIFI_CONNEXION_ESSAIS = 10;
      static const unsigned long UNE_MINUTE            = 60000;
      static const uint16_t      HTTP_PORT             = 80;

      bool       est_en_marche;
      WiFiServer serveur_http;
      Horloge    horloge;
      Vanne      tampon1[NOMBRE_DE_VANNES];
      Vanne      tampon2[NOMBRE_DE_VANNES];
      Vanne *    vannes;
   };
}
