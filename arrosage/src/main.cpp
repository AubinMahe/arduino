#include <Arduino.h>

const unsigned long UNE_MINUTE = 60000;

struct Instant {

   unsigned char heure;  // 0..23
   unsigned char minute; // 0..59
};

static bool operator > ( const Instant & l, const Instant & r ) {
   if( l.heure > r.heure ) {
      return true;
   }
   if( l.heure < r.heure ) {
      return false;
   }
   return l.minute > r.minute;
}

static bool operator < ( const Instant & l, const Instant & r ) {
   if( l.heure < r.heure ) {
      return true;
   }
   if( l.heure < r.heure ) {
      return false;
   }
   return l.minute < r.minute;
}

static Instant maintenant = { 18, 15 };

struct Activite {

   Instant ouverture;
   Instant fermeture;

   bool doitEtreManipulee() const {
      return( maintenant > ouverture )&&( maintenant < fermeture );
   }
};

struct Vanne {

   const uint8_t pin;
   bool          ouverte;
   Activite      matin;
   Activite      soir;

   void evalue() {
      if( ouverte ) {
         if( matin.doitEtreManipulee() || soir.doitEtreManipulee()) {
            ouverte = false;
            digitalWrite( pin, LOW );
            Serial.print( "Fermeture de la vanne n°" );
            Serial.println( pin );
         }
      }
      else {
         if( matin.doitEtreManipulee() || soir.doitEtreManipulee()) {
            ouverte = true;
            digitalWrite( pin, HIGH );
            Serial.print( "Ouverture de la vanne n°" );
            Serial.println( pin );
         }
      }
   }
};

static Vanne vannes[] = {
   { 1, false, {{ 8,  0 },{ 8, 15 }}, {{ 22,  0 },{ 22, 15 }}},
   { 2, false, {{ 8, 15 },{ 8, 30 }}, {{ 22, 15 },{ 22, 30 }}},
   { 3, false, {{ 8, 30 },{ 8, 45 }}, {{ 22, 30 },{ 22, 45 }}},
   { 4, false, {{ 8, 45 },{ 9,  0 }}, {{ 22, 45 },{ 23,  0 }}},
};

static bool est_en_marche;

void setup() {
   est_en_marche = true;
   Serial.begin( 115200 );
}

void loop() {
   if( est_en_marche ) {
      for( unsigned i = 0; i < sizeof(vannes)/sizeof(vannes[0]); ++i ) {
         vannes[i].evalue();
      }
   }
   delay( UNE_MINUTE );
   if( ++maintenant.minute > 59 ) {
      maintenant.minute = 0;
      if( ++maintenant.heure > 23 ) {
         maintenant.heure = 0;
     }
   }
}
