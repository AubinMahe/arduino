#include "Serveur.h"
#include "Commandes.h"
#include "index.html.h"

#include <strings.h>

#define DEMARRER_OU_ARRETER            "Démarrer ou arrêter"
#define METTRE_A_L_HEURE               "Mettre à l'heure"
#define OUVRIR_OU_FERMER_LES_VANNES    "Ouvrir ou fermer les vannes"
#define CHARGER_UNE_CONFIGURATION      "Charger une configuration"
#define LIRE_LA_CONFIGURATION          "Lire la configuration"

#define countof(A) (sizeof(A)/sizeof((A)[0]))

using namespace hpms;

const uint8_t  Serveur::WIFI_CONNEXION_ESSAIS;
const uint16_t Serveur::HTTP_PORT;
bool           Serveur::DUMP = false;

Serveur::Serveur() :
   serveur( HTTP_PORT )
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
      delay( 1500 );
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
   serveur.begin();
   Serial.println( "Serveur HTTP démarré" );
}

json::Status Serveur::mettre_a_l_heure( json::Decoder & parser ) {
   Commande<Instant> mettre_a_l_heure;
   json::Status status( parser.decode( mettre_a_l_heure ));
   if( json::SUCCESS == status ) {
      horloge.actualiser(
         mettre_a_l_heure.argument.get_heure(),
         mettre_a_l_heure.argument.get_minute());
   }
   return status;
}

json::Status Serveur::ouvrir_ou_fermer_les_vannes( json::Decoder & parser ) {
   Serial.println( "Serveur::ouvrir_ou_fermer_les_vannes" );
   static Commande<CommanderLesVannes> commander_les_vannes;
   json::Status status = parser.decode( commander_les_vannes );
   if( json::SUCCESS == status ) {
      for( size_t i = 0; i < countof( commander_les_vannes.argument.les_vannes ); ++i ) {
         CommanderUneVanne & vanne = commander_les_vannes.argument.les_vannes[i];
         if( Vanne::pin_est_valide( vanne.pin )) {
            arrosage.commander_une_vanne( vanne.pin, vanne.ouvrir );
         }
         else {
            break;
         }
      }
   }
   return status;
}

json::Status Serveur::charger_une_configuration( json::Decoder & parser ) {
   Serial.println( "Serveur::charger_une_configuration" );
   Commande<Configuration> configurationComplete;
   json::Status status( parser.decode( configurationComplete ));
   if( json::SUCCESS == status ) {
      arrosage = configurationComplete.argument.arrosage;
      horloge.actualiser(
         configurationComplete.argument.heure.get_heure(),
         configurationComplete.argument.heure.get_minute());
   }
   return status;
}

json::Status Serveur::lire_la_configuration( const char * & response ) {
   static char buffer[1400];
   json::Encoder encoder( buffer, sizeof( buffer ));
   Configuration cfg    ( horloge, arrosage );
   json::Status  status ( encoder.encode( cfg ));
   if( json::SUCCESS == status ) {
      response = buffer;
   }
   else {
      response = 0;
   }
   return status;
}

void Serveur::sendJSonResponse( WiFiClient & client, json::Status status, const char * data ) const {
   static char json[1500];
   if( status == json::SUCCESS ) {
      if( data ) {
         snprintf( json, sizeof( json ), "{\"code\":0,\"data\":%s}", data );
      }
      else {
         snprintf( json, sizeof( json ), "{\"code\":0}" );
      }
   }
   else if( data ) {
      snprintf( json, sizeof( json ), "{\"code\":%d,\"msg\":\"%s\"}", status, data );
   }
   else {
      snprintf( json, sizeof( json ), "{\"code\":%d,\"msg\":\"%s\"}",
         status, json::Decoder::_errMsg );
   }
   client.println( "HTTP/1.1 200 OK" );
   client.println( "Connection: close" );
   client.println( "Content-type: application/json" );
   client.print  ( "Content-Length: " ); client.println( ::strlen( json ));
   client.println();
   client.print( json );
}

void Serveur::handleJSonCommands( WiFiClient & client, json::Decoder & parser ) {
   Commande<DemarrerArreter> commande;
   json::Status              status   = parser.decode( commande );
   const char *              response = 0;
   if( commande.commande[0] ) {
      parser.reset();
      if(      0 == ::strcasecmp( commande.commande, DEMARRER_OU_ARRETER )) {
         arrosage.demarrer( commande.argument.demarrer );
         status = json::SUCCESS;
      }
      else if( 0 == ::strcasecmp( commande.commande, METTRE_A_L_HEURE )) {
         status = mettre_a_l_heure( parser );
      }
      else if( 0 == ::strcasecmp( commande.commande, OUVRIR_OU_FERMER_LES_VANNES )) {
         status = ouvrir_ou_fermer_les_vannes( parser );
      }
      else if( 0 == ::strcasecmp( commande.commande, CHARGER_UNE_CONFIGURATION )) {
         status = charger_une_configuration( parser );
      }
      else if( 0 == ::strcasecmp( commande.commande, LIRE_LA_CONFIGURATION )) {
         status = lire_la_configuration( response );
      }
      else {
         Serial.print  ( "Commande '" );
         Serial.print  ( commande.commande );
         Serial.println( "' inattendue." );
         status   = json::UNEXPECTED_ATTRIBUTE;
         response = "Commande inattendue";
      }
      if( status == json::SUCCESS ) {
         Serial.print  ( "Commande '" );
         Serial.print  ( commande.commande );
         Serial.println( "' exécutée." );
      }
   }
   sendJSonResponse( client, status, response );
}

void Serveur::sendHtmlRootDocument( WiFiClient & client ) const {
   client.println( "HTTP/1.1 200 OK" );
   client.println( "Connection: close" );
   client.println( "Content-type: text/html" );
   client.print  ( "Content-Length: " ); client.println( index_html_len );
   client.println();
   client.println((const char *)index_html );
   Serial.println( "Serveur::sendHtmlRootDocument: " );
}

void Serveur::send404( WiFiClient & client ) const {
   Serial.println( "Serveur::send404" );
   client.println( "HTTP/1.1 404 Not Found" );
   client.println( "Connection: close" );
}

static char buffer[4000];
// 0000: 50 4F 53 54 20 2F 31 39 32 2E 31 36 38 2E 31 2E - POST /192.168.1.
// 1234112312312312312312312312312312312312312312312312312312345678901234561
static char db[40*(4+1+16*3+3+16+1)];

void Serveur::loop() {
   WiFiClient client = serveur.available();
   if( client ) {
      client.setTimeout( 500 );
      while( ! ( client.available() && client.connected())) {
         delay( 4 );
      }
      memset( buffer, 0, sizeof( buffer ));
      size_t receivedBytesCount = client.readBytes( buffer, sizeof( buffer ));
      if( DUMP ) {
         json::Decoder::dump( buffer, receivedBytesCount, db, sizeof( db ));
         Serial.print( receivedBytesCount );
         Serial.println( " octets reçus" );
         Serial.println( db );
      }
      const char * content = 0;
      for( size_t i = 0; i+4 < receivedBytesCount; ++i ) {
         if(  ( buffer[i+0] == 0x0D )
            &&( buffer[i+1] == 0x0A )
            &&( buffer[i+2] == 0x0D )
            &&( buffer[i+3] == 0x0A ))
         {
            content = buffer + i + 4;
            break;
         }
      }
      if( content ) {
         bool         isPost = ( buffer == ::strstr( buffer, "POST /" ));
         const char * type   = ::strcasestr( buffer, "content-type:" );
         if( type ) {
            type += 13;
            while( *type == ' ' ) {
               ++type;
            }
         }
         bool isJSon = type && ( 0 == ::strncasecmp( type, "application/json", 16 ));
         if( isPost && isJSon ) {
            json::Decoder parser( content );
            handleJSonCommands( client, parser );
         }
         else {
            send404( client );
         }
      }
      else if(( buffer == ::strstr( buffer, "GET / "           ))
         ||   ( buffer == ::strstr( buffer, "GET /index.html " )))
      {
         sendHtmlRootDocument( client );
      }
      else {
         send404( client );
      }
      client.flush();
      client.stop();
   }
   else {
      horloge.actualiser();
      arrosage.evaluer( horloge );
   }
   delay( 20 );
}

// Pour des raisons évidentes de sécurité, le code sous Git ne contient pas mes paramètres WiFi.
// Le fichier 'WiFi' contient les deux lignes suivantes, correctement renseignées:
// const char * Arrosage::SSID     = "write your ssid here";
// const char * Arrosage::WIFI_PWD = "write your password here";
#include "WiFi"

static hpms::Serveur theServeur;

#ifdef HPMS_TESTS
void executeAutoTestsIfAny( void );
#endif

void setup() {
   theServeur.setup();
#ifdef HPMS_TESTS
   executeAutoTestsIfAny();
#endif
}

void loop() {
   theServeur.loop();
}
