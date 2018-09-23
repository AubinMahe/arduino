#include "Horloge.h"
#include "Log.h"

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

using namespace hpms;

static const unsigned long PRESQUE_UNE_JOURNEE = 23*60*60*1000; // en ms

Horloge::Horloge():
   demarrage_de_l_autotest( 0 ),
   derniere_mise_a_l_heure( 0 ),
   refHeure ( 0 ),
   refMinute( 0 )
{}

void Horloge::actualiserDepuisNTP( void ) {
   WiFiUDP   ntpUDP;
   NTPClient ntpClient( ntpUDP, "europe.pool.ntp.org", 7200 );
   ntpClient.begin();
   ntpClient.update();
   refHeure  = heure  = ntpClient.getHours();
   refMinute = minute = ntpClient.getMinutes();
   ntpClient.end();
   derniere_mise_a_l_heure = millis();
   demarrage_de_l_autotest = 0;
   Log("Horloge::actualiserDepuisNTP|%02d:%02d", heure, minute );
}

bool Horloge::actualiser( bool autotest /* = false */ ) {
   bool autotest_termine = false;
   if(  ( derniere_mise_a_l_heure == 0 )
      ||( millis() - derniere_mise_a_l_heure > PRESQUE_UNE_JOURNEE ))
   {
      actualiserDepuisNTP();
   }
   else {
      unsigned long ms;
      uint8_t hb = heure;
      if( autotest ) {
         if( ! demarrage_de_l_autotest ) {
            demarrage_de_l_autotest = derniere_mise_a_l_heure = millis();
            hb = heure = minute = refHeure = refMinute = 0;
         }
         ms = 600*( millis() - demarrage_de_l_autotest ); // 24 h ==> 2 m 40 sec
      }
      else {
         ms = millis() - derniere_mise_a_l_heure; // temps écoulé en ms
      }
      unsigned long sec = ( ms / 1000UL ) % 60UL;                        // temps écoulé en s
      unsigned long min = (( ms - 1000UL*sec ) / 1000UL / 60UL ) % 60UL; // temps écoulé en min
      unsigned long heu = (( ms - 1000UL*( sec + 60UL*min )) / 1000UL / 60UL / 60UL ) % 24UL;
      minute = refMinute + min;
      unsigned long h = minute / 60;
      minute %= 60;
      heure  = refHeure  + heu + h;
      heure  %= 24;
      if( autotest && ( heure < hb )) {
         autotest_termine = true;
         actualiserDepuisNTP();
      }
   }
   return autotest_termine;
}

void Horloge::actualiser( uint8_t h, uint8_t m ) {
   heure                   = refHeure  = h;
   minute                  = refMinute = m;
   derniere_mise_a_l_heure = millis();
   Log( "Horloge::actualiser|%02d:%02d", heure, minute );
}
