#pragma once

#include "Arrosage.h"

#include <ESP8266WiFi.h>

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

      void send_json_response  ( WiFiClient & client, json::Status status, const char * buffer = 0 ) const;
      void handle_json_commands( WiFiClient & client, json::Decoder & parser );
#ifndef ESP8266
      void send_file( const char * path, WiFiClient & client ) const;
#endif
      void send_index_html  ( WiFiClient & client ) const;
      void send_arrosage_css( WiFiClient & client ) const;
      void send_arrosage_js ( WiFiClient & client ) const;
      void send_favicon_ico ( WiFiClient & client ) const;
      void send_404         ( WiFiClient & client, const char * request ) const;

   private:

      static const char *   SSID;
      static const char *   WIFI_PWD;
      static const uint8_t  WIFI_CONNEXION_ESSAIS = 10;
      static const uint16_t HTTP_PORT             = 8080;

      WiFiServer serveur;
      Arrosage   arrosage;
   };
}
