#pragma once

#include "Arrosage.h"

#include <ESP8266WiFi.h>

namespace hpms {

   class Serveur {
   public:

      Serveur( void );

      void loop( void );

   private:

      json::Status demarrer                   ( bool demarrer );
      json::Status demarrer_l_auto_test       ( bool demarrer );
      json::Status mettre_a_l_heure           ( json::Decoder & parser );
      json::Status ouvrir_ou_fermer_les_vannes( json::Decoder & parser );
      json::Status lire_la_configuration      ( const char * &  response );
      json::Status charger_une_configuration  ( json::Decoder & parser );

      void send_json_response  ( WiFiClient & client, json::Status status, const char * buffer = 0 ) const;
      void handle_json_commands( WiFiClient & client, json::Decoder & parser );

      void send_index_html( WiFiClient & client ) const;
      void send_404       ( WiFiClient & client ) const;

   private:

      static const char *   SSID;
      static const char *   WIFI_PWD;
      static const uint8_t  WIFI_CONNEXION_ESSAIS = 10;
      static const uint16_t HTTP_PORT             = 8080;
      static /* */ bool     DUMP;

      WiFiServer serveur;
      Arrosage   arrosage;
   };
}
