#include "Serveur.h"
#include "Commandes.h"
#include "Log.h"
#include "Journal.h"

#ifndef ESP8266
#  include <sys/types.h>
#  include <sys/stat.h>
#  include <fcntl.h> // O_RDONLY
#  include <unistd.h>
#endif

#include <strings.h>

#define DEMARRER_OU_ARRETER            "Démarrer ou arrêter"
#define AUTO_TEST                      "Auto-test"
#define LIRE_L_AUTO_TEST               "Lire l'auto-test"
#define METTRE_A_L_HEURE               "Mettre à l'heure"
#define LIRE_L_ETAT_DES_VANNES         "Lire l'état des vannes"
#define OUVRIR_OU_FERMER_LES_VANNES    "Ouvrir ou fermer les vannes"
#define CHARGER_UNE_CONFIGURATION      "Charger une configuration"
#define LIRE_LA_CONFIGURATION          "Lire la configuration"

#include "index.html.h"

using namespace hpms;

const uint8_t  Serveur::WIFI_CONNEXION_ESSAIS;
const uint16_t Serveur::HTTP_PORT;
bool           Serveur::DUMP = false;

static char buffer[4000];

Serveur::Serveur( void ) :
   serveur( HTTP_PORT )
{
   pinMode( 0, OUTPUT );
   pinMode( 1, OUTPUT );
   pinMode( 2, OUTPUT );
   pinMode( 3, OUTPUT );
   Serial.begin( 115200 );
   Log( "Serveur::Serveur|Connexion à %s", SSID );
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
   serveur.begin();
   Log( "Serveur::Serveur|http://%s:8080 est à l'écoute", WiFi.localIP().toString().c_str());
}

json::Status Serveur::demarrer( bool demarrer ) {
   Log( "Serveur::demarrer( %s )", demarrer ? "true" : "false" );
   arrosage.demarrer( demarrer );
   return json::SUCCESS;
}

json::Status Serveur::controler_l_auto_test( bool demarrer ) {
   Log( "Serveur::demarrer_l_auto_test( %s )", demarrer ? "true" : "false" );
   if( demarrer ) {
      arrosage.demarrage_de_l_auto_test();
   }
   else {
      arrosage.arret_de_l_auto_test();
   }
   return json::SUCCESS;
}

json::Status Serveur::lire_l_auto_test( const char * & response ) {
   Log( "Serveur::lire_l_auto_test" );
   if( Journal::le_journal->encode( buffer, sizeof( buffer ), arrosage.est_en_auto_test())) {
      response = buffer;
      return json::SUCCESS;
   }
   response = 0;
   return json::BUFFER_OVERFLOW;
}

json::Status Serveur::mettre_a_l_heure( json::Decoder & parser ) {
   Log( "Serveur::mettre_a_l_heure" );
   Commande<Instant> mettre_a_l_heure;
   json::Status status( parser.decode( mettre_a_l_heure ));
   if( json::SUCCESS == status ) {
      arrosage.actualiser(
         mettre_a_l_heure.argument.get_heure(),
         mettre_a_l_heure.argument.get_minute());
   }
   return status;
}

json::Status Serveur::lire_l_etat_des_vannes( const char * & response ) {
   Log( "Serveur::lire_l_etat_des_vannes" );
   json::Encoder encoder( buffer, sizeof( buffer ));
   EtatDesVannes etat( arrosage );
   json::Status  status( encoder.encode( etat ));
   if( json::SUCCESS == status ) {
      response = buffer;
   }
   else {
      response = 0;
   }
   return status;
}

json::Status Serveur::ouvrir_ou_fermer_les_vannes( json::Decoder & parser ) {
   Log( "Serveur::ouvrir_ou_fermer_les_vannes" );
   Commande<CommanderLesVannes> commander_les_vannes;
   json::Status status = parser.decode( commander_les_vannes );
   if( json::SUCCESS == status ) {
      for( size_t i = 0; i < countof( commander_les_vannes.argument.les_vannes ); ++i ) {
         CommanderUneVanne & vanne = commander_les_vannes.argument.les_vannes[i];
         if(  ( 0 <= vanne.pin )&&( vanne.pin < arrosage.NBR_VANNES )
            &&( vanne.etat >= Vanne::PREMIER_ETAT )&&( vanne.etat <= Vanne::DERNIER_ETAT ))
         {
            arrosage.commander_une_vanne( vanne.pin, vanne.etat );
         }
      }
   }
   return status;
}

json::Status Serveur::lire_la_configuration( const char * & response ) {
   Log( "Serveur::lire_la_configuration" );
   json::Encoder encoder( buffer, sizeof( buffer ));
   Configuration cfg    ( arrosage );
   json::Status  status ( encoder.encode( cfg ));
   if( json::SUCCESS == status ) {
      response = buffer;
   }
   else {
      response = 0;
   }
   return status;
}

json::Status Serveur::charger_une_configuration( json::Decoder & parser ) {
   Log( "Serveur::charger_une_configuration" );
   Commande<Configuration> configurationComplete;
   json::Status status( parser.decode( configurationComplete ));
   if( json::SUCCESS == status ) {
      arrosage = configurationComplete.argument.arrosage;
      arrosage.a_ete_actualise();
   }
   return status;
}

void Serveur::send_json_response( WiFiClient & client, json::Status status, const char * data ) const {
   Log( "Serveur::send_json_response" );
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
      json::Decoder::updateErrorMessage( status );
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

void Serveur::handle_json_commands( WiFiClient & client, json::Decoder & parser ) {
   Log( "Serveur::handle_json_commands" );
   Commande<DemarrerArreter> commande;
   json::Status              status   = parser.decode( commande );
   const char *              response = 0;
   if( commande.commande[0] ) {
      if(  arrosage.est_en_auto_test()
         &&((  ::strcasecmp( commande.commande, AUTO_TEST        )
            && ::strcasecmp( commande.commande, LIRE_L_AUTO_TEST ))
            || commande.argument.demarrer                          ))
      {
         send_json_response( client, json::UNEXPECTED,
            "Commande non recevable: auto-test en cours" );
         return;
      }
      parser.reset();
      if(      0 == ::strcasecmp( commande.commande, DEMARRER_OU_ARRETER )) {
         status = demarrer( commande.argument.demarrer );
      }
      else if( 0 == ::strcasecmp( commande.commande, AUTO_TEST )) {
         if( ! arrosage.est_en_auto_test() && ! commande.argument.demarrer ) {
            send_json_response( client, json::UNEXPECTED,
               "Commande non recevable: l'auto-test n'est pas en cours" );
         }
         else {
            status = controler_l_auto_test( commande.argument.demarrer );
         }
      }
      else if( 0 == ::strcasecmp( commande.commande, LIRE_L_AUTO_TEST )) {
         status = lire_l_auto_test( response );
      }
      else if( 0 == ::strcasecmp( commande.commande, METTRE_A_L_HEURE )) {
         status = mettre_a_l_heure( parser );
      }
      else if( 0 == ::strcasecmp( commande.commande, LIRE_L_ETAT_DES_VANNES )) {
         status = lire_l_etat_des_vannes( response );
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
         status   = json::UNEXPECTED;
         response = "Commande inattendue";
         Log( "Commande '%s' inattendue.", commande.commande );
      }
      if( status == json::SUCCESS ) {
         Log( "Commande '%s' exécutée ou en cours d'exécution.", commande.commande );
      }
   }
   send_json_response( client, status, response );
}

void Serveur::send_index_html( WiFiClient & client ) const {
   Log( "Serveur::send_index_html" );
   client.println( "HTTP/1.1 200 OK" );
   client.println( "Connection: close" );
   client.println( "Content-type: text/html" );
#ifdef ESP8266
   client.print  ( "Content-Length: " ); client.println( index_html_len );
   client.println();
   client.print((const char *)index_html );
#else
   int page = open( "index.html", O_RDONLY );
   if( page ) {
      struct stat s;
      ::fstat( page, &s );
      client.print( "Content-Length: " ); client.println( s.st_size );
      client.println();
      int count = 0;
      while( 0 != ( count = read( page, buffer, sizeof( buffer ) - 1 ))) {
         buffer[count] = '\0';
         client.print( buffer );
      }
      close( page );
   }
   else {
      perror( "index.html" );
   }
#endif
}

void Serveur::send_404( WiFiClient & client ) const {
   Log( ": Serveur::send_404" );
   client.println( "HTTP/1.1 404 Not Found" );
   client.println( "Connection: close" );
}

const char * strcasestr( const char * haystack, const char * needle ) {
   for( size_t h = 0, hlen = ::strlen( haystack ), nlen = ::strlen( needle ); h < hlen; ++h ) {
      const char * retVal = haystack + h;
      for( size_t n = 0; retVal && n < nlen; ++n ) {
         if( tolower( retVal[n] ) != tolower( needle[n] )) {
            retVal = 0;
         }
      }
      if( retVal ) {
         return retVal;
      }
   }
   return 0;
}

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
      Log( "Serveur::loop|%d octets reçus.", receivedBytesCount );
      if( DUMP ) {
         json::Decoder::dump( buffer, receivedBytesCount, db, sizeof( db ));
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
            handle_json_commands( client, parser );
         }
         else {
            send_404( client );
         }
      }
      else if(( buffer == ::strstr( buffer, "GET / "           ))
         ||   ( buffer == ::strstr( buffer, "GET /index.html " )))
      {
         send_index_html( client );
      }
      else {
         send_404( client );
      }
      client.flush();
      client.stop();
   }
   else {
      arrosage.evaluer();
   }
   delay( 20 );
}

// Pour des raisons évidentes de sécurité, le code sous Git ne contient pas mes paramètres WiFi.
// Le fichier 'WiFi' contient les deux lignes suivantes, correctement renseignées:
// const char * Arrosage::SSID     = "write your ssid here";
// const char * Arrosage::WIFI_PWD = "write your password here";
#include "WiFi"

static Serveur * theServeur = 0;

#ifdef HPMS_TESTS
void executeAutoTestsIfAny( void );
#endif

void setup( void ) {
   theServeur = new Serveur();
#ifdef HPMS_TESTS
   executeAutoTestsIfAny();
#endif
}

void loop( void ) {
   theServeur->loop();
}

void tear_down( void ) {
   delete theServeur;
   theServeur = 0;
}
