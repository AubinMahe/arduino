#pragma once

#include <ESP8266WiFi.h>

#include "Horloge.h"
#include "Arrosage.h"

namespace hpms {

   class Serveur {
   public:

      Serveur();

   public:

      void setup();

      void loop();

   private:

      void sendHTTPResponse( WiFiClient & client, json::Status status, const char * buffer = 0 ) const;

      /**
       * La configuration retournée est un objet encodé en JSON de même type
       * que celui utilisé pour configurer().
       */
      void lire_la_configuration( WiFiClient & client ) const;

      /**
       * L'objet encodé en JSON à envoyer par HTTP est :
       *        {est_en_marche: <boolean>}
       */
      void demarrer_arreter( WiFiClient & client );

      /**
       * L'objet encodé en JSON à envoyer par HTTP est :
       *        {"vannes":
       *           [{"pin": <integer>, "ouverte": <boolean>}
       *           ,{"pin": <integer>, "ouverte": <boolean>}
       *           ...
       *           ]
       *        }
       */
      void commander_les_vannes( WiFiClient & client );

      /**
       * L'objet encodé en JSON à envoyer par HTTP est :
       *        {"est_en_marche": <boolean>
       *        ,"vannes":
       *           [{"pin": <integer>
       *            ,"ouverte": <boolean>
       *            ,"matin":
       *               {"ouverture": {"heure": <integer>, "minute": <integer>}
       *               ,"fermeture": {"heure": <integer>, "minute": <integer>}
       *               }
       *            ,"soir":
       *               {"ouverture": {"heure": <integer>, "minute": <integer>}
       *               ,"fermeture": {"heure": <integer>, "minute": <integer>}
       *               }
       *            }
       *           ,{"pin": <integer>
       *            ,"ouverte": <boolean>
       *            ,"matin":
       *               {"ouverture": {"heure": <integer>, "minute": <integer>}
       *               ,"fermeture": {"heure": <integer>, "minute": <integer>}
       *               }
       *            ,"soir":
       *               {"ouverture": {"heure": <integer>, "minute": <integer>}
       *               ,"fermeture": {"heure": <integer>, "minute": <integer>}
       *               }
       *            },
       *            ...
       *           ]
       *        }
       */
      void configurer( WiFiClient & client );

   private:

      static const char *        SSID;
      static const char *        WIFI_PWD;

      static const char          LIRE_LA_CONFIGURATION = '1';
      static const char          DEMARRER_ARRETER      = '2';
      static const char          CONFIGURER_LE_SYSTEME = '3';
      static const char          COMMANDER_LES_VANNES  = '4';

      static const char          MARCHE                = 'm';
      static const char          ARRET                 = 'a';

      static const char          FERMER_LA_VANNE       = 'f';
      static const char          OUVRIR_LA_VANNE       = 'o';

      static const unsigned long WIFI_CONNEXION_ESSAIS = 10;
      static const unsigned long UNE_MINUTE            = 60000;
      static const uint16_t      HTTP_PORT             = 80;

      WiFiServer serveur_http;
      Horloge    horloge;
      Arrosage   arrosage;
   };
}
