#include <Arduino.h>

enum Etat_t {
   ETA_VEILLE,
   ETA_ACTIF,
   ETA_ATTENTE_1,
   ETA_LED_1,
   ETA_ATTENTE_2,
   ETA_LED_2,
   ETA_ATTENTE_3,
   ETA_LED_3,
};

static const uint8_t       BOUTON_PIN                     = 2;
static const uint8_t       LED1_PIN                       = 3;
static const uint8_t       LED2_PIN                       = 4;
static const uint8_t       LED3_PIN                       = 5;
static const uint8_t       BUZZER_PIN                     = 7;
static const unsigned long FREQUENCE_DU_BUZZER            = 220;
static const unsigned long DUREE_DU_BUZZER                = 160;
static const unsigned long PERIODE_D_ACTIVATION_DU_BUZZER = 500UL;
static const unsigned long DEBUT_DE_FENETRE_1             = 3000;
static const unsigned long FIN_DE_FENETRE_1               = 5000;
static const unsigned long DEBUT_DE_FENETRE_2             = 7000;
static const unsigned long FIN_DE_FENETRE_2               = 9000;
static const unsigned long DEBUT_DE_FENETRE_3             = 12000;
static const unsigned long FIN_DE_FENETRE_3               = 14000;

static Etat_t        etat          = ETA_VEILLE;
static bool          enfonce       = false;
static unsigned long t0            = 0UL;
static unsigned long buzzer_t0     = 0UL;
static unsigned long buzzer_cycle  = 0UL;
static bool          led_3_a_faire = true;

static void desactive_le_buzzer() {
   // Serial.println( "desactive_le_buzzer" );
   noTone( BUZZER_PIN );
}

static void active_le_buzzer() {
   // Serial.println( "active_le_buzzer" );
   tone( BUZZER_PIN, FREQUENCE_DU_BUZZER );
   buzzer_t0 = millis();
}

static void demarre() {
   Serial.println( "demarre" );
   t0 = millis();
}

static void eteint_tout() {
   Serial.println( "eteint_tout" );
   t0           = 0;
   buzzer_t0    = 0UL;
   buzzer_cycle = 0UL;
   desactive_le_buzzer();
   digitalWrite( LED1_PIN, LOW );
   digitalWrite( LED2_PIN, LOW );
   digitalWrite( LED3_PIN, LOW );
}

static void allume_une_LED() {
   Serial.println( "allume_une_LED" );
   digitalWrite( LED1_PIN, HIGH );
   digitalWrite( LED2_PIN, LOW );
   digitalWrite( LED3_PIN, LOW );
}

static void allume_deux_LED() {
   Serial.println( "allume_deux_LED" );
   digitalWrite( LED1_PIN, HIGH );
   digitalWrite( LED2_PIN, HIGH );
   digitalWrite( LED3_PIN, LOW );
}

static void affiche_l_indice() {
   Serial.println( "affiche_l_indice" );
}

static void allume_trois_LED() {
   Serial.println( "allume_trois_LED" );
   digitalWrite( LED1_PIN, HIGH );
   digitalWrite( LED2_PIN, HIGH );
   digitalWrite( LED3_PIN, HIGH );
   affiche_l_indice();
}

static void veille() {
   if( digitalRead( BOUTON_PIN ) ) {
      enfonce = true;
   } else if( enfonce ) { // on vient de relâcher le bouton
      enfonce = false;
      etat    = ETA_ACTIF;
      demarre();
   }
}

static void actif( unsigned long temps_ecoule ) {
   if( digitalRead( BOUTON_PIN ) ) {
      enfonce = true;
   } else if( enfonce ) { // on vient de relâcher le bouton
      enfonce = false;
      etat    = ETA_VEILLE;
      eteint_tout();
   } else if( temps_ecoule >= DEBUT_DE_FENETRE_1 ) {
      etat = ETA_ATTENTE_1;
      Serial.println( "Appui attendu" );
   }
}

static void attente( unsigned long temps_ecoule,
                     Etat_t        suivant,
                     unsigned long echeance,
                     void ( *action )() ) {
   if( digitalRead( BOUTON_PIN ) ) {
      enfonce = true;
   } else if( enfonce ) { // on vient de relâcher le bouton
      enfonce       = false;
      etat          = suivant;
      led_3_a_faire = true;
      action();
   } else if( temps_ecoule >= echeance ) {
      etat = ETA_VEILLE;
      eteint_tout();
   }
}

static void led( unsigned long temps_ecoule, Etat_t suivant, unsigned long echeance ) {
   if( digitalRead( BOUTON_PIN ) ) {
      enfonce = true;
   } else if( enfonce ) { // on vient de relâcher le bouton
      enfonce = false;
      etat    = ETA_VEILLE;
      eteint_tout();
   } else if( temps_ecoule >= echeance ) {
      etat = suivant;
      Serial.println( "Appui attendu" );
   }
}

static void led_3() {
   if( digitalRead( BOUTON_PIN ) ) {
      enfonce = true;
   } else if( enfonce ) { // on vient de relâcher le bouton
      enfonce = false;
      etat    = ETA_VEILLE;
      eteint_tout();
   } else if( led_3_a_faire ) {
      led_3_a_faire = false;
      allume_trois_LED();
      Serial.println( "Appui attendu pour reset" );
   }
}

static void initialise_la_liaison_serie() {
   Serial.begin( 115200 );
}

static void initialise_les_entrees_sorties() {
   pinMode( BOUTON_PIN, INPUT );
   pinMode( LED1_PIN, OUTPUT );
   pinMode( LED2_PIN, OUTPUT );
   pinMode( LED3_PIN, OUTPUT );
   pinMode( BUZZER_PIN, OUTPUT );
}

void setup() {
   Serial.println( "setup" );
   initialise_la_liaison_serie();
   initialise_les_entrees_sorties();
}

void loop() {
   if( buzzer_t0 ) {
      unsigned long temps_ecoule = millis() - buzzer_t0;
      if( temps_ecoule >= DUREE_DU_BUZZER ) {
         desactive_le_buzzer();
         buzzer_t0 = 0;
      }
   }
   unsigned long temps_ecoule = millis() - t0;
   if( t0 && ( temps_ecoule / PERIODE_D_ACTIVATION_DU_BUZZER ) > buzzer_cycle ) {
      buzzer_cycle = temps_ecoule / PERIODE_D_ACTIVATION_DU_BUZZER;
      active_le_buzzer();
   }
   switch( etat ) {
   case ETA_VEILLE: veille(); break;
   case ETA_ACTIF: actif( temps_ecoule ); break;
   case ETA_ATTENTE_1: attente( temps_ecoule, ETA_LED_1, FIN_DE_FENETRE_1, &allume_une_LED ); break;
   case ETA_LED_1: led( temps_ecoule, ETA_ATTENTE_2, DEBUT_DE_FENETRE_2 ); break;
   case ETA_ATTENTE_2:
      attente( temps_ecoule, ETA_LED_2, FIN_DE_FENETRE_2, &allume_deux_LED );
      break;
   case ETA_LED_2: led( temps_ecoule, ETA_ATTENTE_3, DEBUT_DE_FENETRE_3 ); break;
   case ETA_ATTENTE_3:
      attente( temps_ecoule, ETA_LED_3, FIN_DE_FENETRE_3, &allume_deux_LED );
      break;
   case ETA_LED_3: led_3(); break;
   }
}
