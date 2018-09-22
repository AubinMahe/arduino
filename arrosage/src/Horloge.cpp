#include "Horloge.h"

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

using namespace hpms;

static const unsigned long PRESQUE_UNE_JOURNEE = 23*60*60*1000; // en ms

Horloge::Horloge():
   derniere_mise_a_l_heure( 0 ),
   refHeure ( 0 ),
   refMinute( 0 )
{}

static void debugHorloge( uint8_t heure, uint8_t minute ) {
   Serial.print("Horloge::actualiser|Il est " );
   if( heure < 10 ) {
      Serial.print( "0" );
   }
   Serial.print( heure );
   Serial.print( ":" );
   if( minute < 10 ) {
      Serial.print( "0" );
   }
   Serial.println( minute );
}

void Horloge::actualiser() {
   if(  ( derniere_mise_a_l_heure == 0 )
      ||( millis() - derniere_mise_a_l_heure > PRESQUE_UNE_JOURNEE ))
   {
      Serial.println( "Horloge::actualiser|Mise à jour de l'heure par NTP." );
      WiFiUDP   ntpUDP;
      NTPClient ntpClient( ntpUDP, "europe.pool.ntp.org", 7200 );
      ntpClient.begin();
      ntpClient.update();
      refHeure  = heure  = ntpClient.getHours();
      refMinute = minute = ntpClient.getMinutes();
      ntpClient.end();
      derniere_mise_a_l_heure = millis();
      debugHorloge( heure, minute );
   }
   else {
      unsigned long ms  = millis() - derniere_mise_a_l_heure;            // temps écoulé en ms
      unsigned long sec = ( ms / 1000UL ) % 60UL;                        // temps écoulé en s
      unsigned long min = (( ms - 1000UL*sec ) / 1000UL / 60UL ) % 60UL; // temps écoulé en min
      unsigned long heu = (( ms - 1000UL*( sec - 60UL*min )) / 1000UL / 60UL / 60UL ) % 24UL;
      minute = refMinute + min;
      unsigned long h = minute / 60;
      minute %= 60;
      heure  = refHeure  + heu + h;
      heure  %= 24;
   }
}

void Horloge::actualiser( uint8_t h, uint8_t m ) {
   heure  = refHeure  = h;
   minute = refMinute = m;
   derniere_mise_a_l_heure = millis();
   debugHorloge( heure, minute );
}
