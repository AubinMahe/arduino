#include "Horloge.h"

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

using namespace hpms;

void Horloge::actualiser() {
   if( millis() - derniere_mise_a_l_heure > UNE_JOURNEE ) {
      Serial.println( "Mise à jour de l'heure par NTP." );
      WiFiUDP   ntpUDP;
      NTPClient ntpClient( ntpUDP, "europe.pool.ntp.org", 3600, 60000 );
      ntpClient.begin();
      ntpClient.update();
      refHeure  = heure  = ntpClient.getHours();
      refMinute = minute = ntpClient.getMinutes();
      derniere_mise_a_l_heure = millis();
   }
   else {
      unsigned long ms  = millis() - derniere_mise_a_l_heure;            // temps écoulé en ms
      unsigned long sec = ( ms / 1000UL ) % 60UL;                        // temps écoulé en s
      unsigned long min = (( ms - 1000UL*sec ) / 1000UL / 60UL ) % 60UL; // temps écoulé en min
      unsigned long heu = (( ms - 1000UL*( sec - 60UL*min )) / 1000UL / 60UL / 60UL ) % 24UL;
      heure  = refHeure  + min;
      minute = refMinute + heu;
   }
}
