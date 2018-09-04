#include "Serveur.h"

struct DemarrerArreter : private json::IJSonData {

   DemarrerArreter() :
      demarrer( false )
   {}

   virtual json::Status decode( const char * name, json::Decoder & decoder ) {
      if( 0 == strcmp( name, "demarrer" )) {
         return decoder.get( demarrer );
      }
      return json::UNEXPECTED_ATTRIBUTE;
   }

   virtual json::Status encode( json::Encoder & encoder ) const {
      return json::SUCCESS;
   }

   bool demarrer;
};

struct CommanderUneVanne : private json::IJSonData {

   CommanderUneVanne() :
      pin( 0 ),
      ouvrir( false )
   {}

   json::Status decode( const char * name, json::Decoder & decoder ) {
      if( 0 == strcmp( name, "pin" )) {
         return decoder.get( pin );
      }
      if( 0 == strcmp( name, "ouvrir" )) {
         return decoder.get( ouvrir );
      }
      return json::UNEXPECTED_ATTRIBUTE;
   }

   virtual json::Status encode( json::Encoder & encoder ) const {
      return json::SUCCESS;
   }

   uint8_t pin;
   bool    ouvrir;
};

struct CommanderLesVannes : private json::IJSonData {

   CommanderLesVannes() {}

   json::Status decode( const char * name, json::Decoder & decoder ) {
      if( 0 == strcmp( name, "vannes" )) {
         return decoder.decode( vannes, sizeof( vannes )/sizeof( vannes[0] ));
      }
      return json::UNEXPECTED_ATTRIBUTE;
   }

   virtual json::Status encode( json::Encoder & encoder ) const {
      return json::SUCCESS;
   }

   CommanderUneVanne vannes[100];
};

using namespace hpms;

Serveur::Serveur() :
   serveur_http( HTTP_PORT )
{}

void Serveur::setup() {
   Serial.begin( 115200 );
   Serial.println();
   Serial.println();
   Serial.print( "Connexion à " );
   Serial.println( SSID );
   WiFi.begin( SSID, WIFI_PWD );
   unsigned essai = 0;
   while( WiFi.status() != WL_CONNECTED ) {
      delay( 500 );
      Serial.print( "." );
      if( ++essai > WIFI_CONNEXION_ESSAIS ) {
         Serial.println( "Impossible de joindre le réseau WiFi." );
         ESP.deepSleep( 10e6 );
      }
   }
   Serial.println();
   Serial.println( "WiFi connecté." );
   Serial.println( "Adresse IP : " );
   Serial.println( WiFi.localIP());
   serveur_http.begin();
   Serial.println( "Serveur HTTP démarré" );
}

void Serveur::loop() {
   horloge.actualiser();
   WiFiClient client = serveur_http.available();
   if( client ) {
      while( ! client.available()) {
         delay(1);
      }
      char header;
      if( client.readBytes( &header, 1 ) == 1 ) {
         Serial.println( "-----------------------------" );
         Serial.print( "Commande reçue : '" );
         Serial.print( header );
         Serial.println( "'" );
         switch( header ) {
         case LIRE_LA_CONFIGURATION: lire_la_configuration( client ); break;
         case DEMARRER_ARRETER     : demarrer_arreter     ( client ); break;
         case CONFIGURER_LE_SYSTEME: configurer           ( client ); break;
         case COMMANDER_LES_VANNES : commander_les_vannes ( client ); break;
         }
      }
      client.flush();
   }
   arrosage.evaluer( horloge );
   delay( UNE_MINUTE );
}

void Serveur::sendHTTPResponse( WiFiClient & client, json::Status status, const char * buffer ) const {
   client.print( "HTTP/1.1 200 OK\r\n" );
   client.print( "Content-type: application/json\r\n" );
   client.print( "Connection: close\r\n" );
   client.print( "\r\n" );
   client.print( "{\"code\":" );
   client.print( status );
   if( status == json::SUCCESS ) {
      client.print( ",\"msg\":\"SUCCESS\"" );
      if( buffer ) {
         client.print( ",\"data\":" );
         client.print( buffer );
      }
   }
   else {
      client.print( ",\"msg\":\"internal server error: JSON encoding failed\"" );
   }
   client.print( "}" );
   client.print( "\r\n" );
}

void Serveur::lire_la_configuration( WiFiClient & client ) const {
   char buffer[10*1024];
   json::Encoder generator( buffer, sizeof( buffer ));
   json::Status status = generator.encode( arrosage );
   sendHTTPResponse( client, status, buffer );
}

void Serveur::demarrer_arreter( WiFiClient & client ) {
   char payload[1024];
   if( client.readBytes( payload, sizeof( payload )) > 0 ) {
      json::Decoder parser( payload );
      DemarrerArreter commande;
      json::Status status = parser.decode( commande );
      sendHTTPResponse( client, status );
   }
}

void Serveur::commander_les_vannes( WiFiClient & client ) {
   char payload[1024];
   if( client.readBytes( payload, sizeof( payload )) > 0 ) {
      json::Decoder parser( payload );
      CommanderLesVannes commande;
      json::Status status = parser.decode( commande );
      if( json::SUCCESS == status ) {
         for( size_t i = 0; i < sizeof(commande.vannes)/sizeof(commande.vannes[0]); ++i ) {
            uint8_t pin = commande.vannes[0].pin;
            if( Vanne::pin_est_valide( pin )) {
               arrosage.commander_une_vanne( pin, commande.vannes[0].ouvrir );
            }
            else {
               break;
            }
         }
      }
      sendHTTPResponse( client, status );
   }
}

void Serveur::configurer( WiFiClient & client ) {
   static char payload[1024];
   if( client.readBytes( payload, sizeof( payload )) > 0 ) {
      json::Decoder parser( payload );
      Arrosage commande;
      json::Status status = parser.decode( commande );
      if( json::SUCCESS == status ) {
         arrosage = commande;
      }
      sendHTTPResponse( client, status );
   }
}

// Pour des raisons évidentes de sécurité, le code sous Git ne contient pas mes paramètres WiFi.
// Le fichier 'WiFi' contient les deux lignes suivantes, correctement renseignées:
// const char * Arrosage::SSID     = "write your ssid here";
// const char * Arrosage::WIFI_PWD = "write your password here";
#include "WiFi"

static hpms::Serveur theServeur;

void setup() {
   theServeur.setup();
}

void loop() {
   theServeur.loop();
}
