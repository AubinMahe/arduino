#pragma once

#include "Arrosage.h"

#include <ESP8266WiFi.h>
#include <FS.h>

namespace hpms {

   class Serveur {
   public:

      static bool HTTP_DUMP;

   public:

      Serveur( void );

      void loop( void );

   private:

      json::Status demarrer                   ( bool            demarrer );
      json::Status controler_l_auto_test      ( bool            demarrer );
      json::Status lire_l_auto_test           ( const char * &  response );
      json::Status mettre_a_l_heure           ( json::Decoder & parser   );
      json::Status lire_l_etat_des_vannes     ( const char * &  response );
      json::Status ouvrir_ou_fermer_les_vannes( json::Decoder & parser   );
      json::Status lire_la_configuration      ( const char * &  response );
      json::Status charger_une_configuration  ( json::Decoder & parser   );

      void send_json_response( WiFiClient & client, json::Status status, const char * buffer = 0 ) const;
      void handle_json_commands( WiFiClient & client, json::Decoder & parser );
      void send_file( const char * path, const char * contentType, WiFiClient & client ) const;
      void send_404( WiFiClient & client, const String & request ) const;

   private:

      static const char *   SSID;
      static const char *   WIFI_PWD;
      static const uint8_t  WIFI_CONNEXION_ESSAIS = 10;
      static const uint16_t HTTP_PORT             = 8080;

      WiFiServer serveur;
      Arrosage   arrosage;
   };
}
