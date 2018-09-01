#include "Arrosage.h"

using namespace hpms;

void Arrosage::commander_une_vanne( WiFiClient & client ) {
   char payload[1024];
   int taille_du_message = client.readBytes( payload, sizeof( payload ));
   // "pp,vv:x,vv:x,vv:x,..."
   if( taille_du_message >= 8 ) {
      int nbr_de_vannes =
         ( payload[0] == ' ' ? 0 : ( 10*( payload[0] - '0' ))) +
         ( payload[1] - '0' );
      if(  ( nbr_de_vannes > 0 )&&( nbr_de_vannes <= NOMBRE_DE_VANNES )
         &&( payload[2] == ',' )
         &&((( taille_du_message - 3 ) / 5 ) == nbr_de_vannes ))
      {
         char * s = payload + 3; // "pp,".length = 3
         for( int i = 0; i < nbr_de_vannes; ++i, s += 5 ) { // "vv:x,".length = 5
            const int vanne =
               ( s[0] == ' ' ? 0 : ( 10*( s[0] - '0' ))) +
               ( s[1] - '0' );
            const char etat = s[3];
            if(  ( s[2] == ':' )
               &&( vanne > 0 )&&( vanne < NOMBRE_DE_VANNES )
               &&(( etat == FERMER_LA_VANNE )||( etat == OUVRIR_LA_VANNE )))
            {
               if( etat == FERMER_LA_VANNE ) {
                  Serial.print( "Fermer la vanne : " );
                  Serial.println( vanne );
                  vannes[vanne].ouvrir();
               }
               else {
                  Serial.print( "Ouvrir la vanne : " );
                  Serial.println( vanne );
                  vannes[vanne].ouvrir();
               }
            }
            else {
               Serial.println( "La commande reçue est incohérente : " );
               Serial.print( "Le numéro de vanne doit être en 1 et " );
               Serial.print( NOMBRE_DE_VANNES );
               Serial.print( " alors qu'il vaut " );
               Serial.println( vanne );
               Serial.print( "s[2], deux points attendu : " );
               Serial.println( s[2] );
               Serial.print( "s[3], {'f','o'} attendu : " );
               Serial.println( s[3] );
               Serial.print( "payload : '" );
               Serial.print( payload );
               Serial.println( "'" );
               break;
            }
         }
      }
      else {
         Serial.println( "La commande reçue est incohérente : " );
         Serial.print( "nbr_de_vannes : " );
         Serial.println( nbr_de_vannes );
         Serial.print( "taille_de_message : " );
         Serial.println( taille_du_message );
         Serial.print( "payload[2], virgule attendue : " );
         Serial.println( payload[2] );
         Serial.print( "payload : '" );
         Serial.print( payload );
         Serial.println( "'" );
      }
   }
}

void Arrosage::configurer( WiFiClient & client ) {
   static char payload[1024];
   int taille_du_message = client.readBytes( payload, sizeof( payload ));
   payload[taille_du_message] = '\0';
   if( taille_du_message > 5 ) {
      Serial.print( "Configuration reçue : " );
      Serial.println( payload );
      int nbr_de_vannes =
         ( payload[0] == ' ' ? 0 : ( 10*( payload[0] - '0' ))) +
         ( payload[1] - '0' );
      bool sont_coherentes =
           ( nbr_de_vannes > 0 )&&( nbr_de_vannes <= (int)NOMBRE_DE_VANNES )
         &&((( taille_du_message - 5 ) / (3*8)) == nbr_de_vannes )
         &&(  payload[2] == ',' )
         &&(( payload[3] == MARCHE )||( payload[3] == ARRET ))
         &&(  payload[4] == ',' );
      Vanne * target = ( vannes == tampon1 ) ? tampon2 : tampon1;
      char *  s      = payload + 5; // "pp,e,".length = 5
      for( int i = 0; sont_coherentes && i < nbr_de_vannes; ++i, s += 8*3 ) {
         sont_coherentes = ( s[8*3-1] == ',' ) && target[i].decode( s );
      }
      if( sont_coherentes ) {
         Serial.println( "Nouvelle configuration chargée." );
         vannes = ( vannes == tampon1 ) ? tampon2 : tampon1;
         est_en_marche = ( payload[2] == MARCHE );
         Serial.print( "Configuration en marche : " );
         Serial.println( est_en_marche ? "OUI" : "non" );
      }
      else {
         Serial.println( "La configuration reçue est incohérente : " );
         Serial.print( "nbr_de_vannes : " );
         Serial.println( nbr_de_vannes );
         Serial.print( "taille_de_message : " );
         Serial.println( taille_du_message );
         Serial.print( "payload[2], virgule attendue : " );
         Serial.println( payload[2] );
         Serial.print( "payload[3], {'m','a'} attendue : " );
         Serial.println( payload[3] );
         Serial.print( "payload[4], virgule attendue : " );
         Serial.println( payload[4] );
         Serial.print( "payload : '" );
         Serial.print( payload );
         Serial.println( "'" );
      }
   }
   else {
      Serial.print( "Mauvaise configuration reçue, " );
      Serial.print( taille_du_message );
      Serial.print( " octet" );
      if( taille_du_message > 1 ) {
         Serial.print( "s" );
      }
      Serial.print( " : '" );
      Serial.print( payload );
      Serial.println( "'" );
   }
}

void Arrosage::setup() {
   est_en_marche = true;
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

void Arrosage::loop() {
   horloge.actualiser();
   WiFiClient client = serveur_http.available();
   if( client ) {
      while( ! client.available()) {
         delay(1);
      }
      char header;
      if( client.readBytes( &header, 1 ) == 1 ) {
         switch( header ) {
         case CONFIGURER: configurer         ( client ); break;
         case COMMANDER : commander_une_vanne( client ); break;
         }
      }
      client.flush();
   }
   if( est_en_marche ) {
      for( unsigned i = 0; i < NOMBRE_DE_VANNES; ++i ) {
         vannes[i].evaluer( horloge );
      }
   }
   delay( UNE_MINUTE );
}

// Pour des raisons évidentes de sécurité, le code sous Git ne contient pas mes paramètres WiFi.
// Le fichier 'WiFi' contient les deux lignes suivantes, correctement renseignées:
// const char * Arrosage::SSID     = "write your ssid here";
// const char * Arrosage::WIFI_PWD = "write your password here";
#include "WiFi"
