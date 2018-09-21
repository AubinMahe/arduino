#pragma once

#include "Horloge.h"
#include "Arrosage.h"

#include <ESP8266WiFi.h>

namespace hpms {

   class Serveur {
   public:

      Serveur();

   public:

      void setup();

      void loop();

   private:

      json::Status charger_une_configuration          ( json::Decoder & parser );
      json::Status lire_la_configuration ( const char * & response );
      json::Status mettre_a_l_heure    ( json::Decoder & parser );
      json::Status ouvrir_ou_fermer_les_vannes( json::Decoder & parser );

      void sendJSonResponse  ( WiFiClient & client, json::Status status, const char * buffer = 0 ) const;
      void handleJSonCommands( WiFiClient & client, json::Decoder & parser );

      void sendHtmlRootDocument( WiFiClient & client ) const;
      void send404             ( WiFiClient & client ) const;

   private:

      static const char *   SSID;
      static const char *   WIFI_PWD;
      static const uint8_t  WIFI_CONNEXION_ESSAIS = 10;
      static const uint16_t HTTP_PORT             = 8080;
      static /* */ bool     DUMP;

      WiFiServer serveur;
      Horloge    horloge;
      Arrosage   arrosage;
   };
}
