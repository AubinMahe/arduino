#include <ActionsDifferees.hpp>
#include <Bouton.hpp>
#include <Automate.hpp>

#include <Arduino.h>

void initialise_la_liaison_serie() {
   Serial.begin( 115200 );
   Serial.println("setup");
}

const uint8_t BOUTON_PIN = 2;
const uint8_t LED1_PIN   = 3;
const uint8_t LED2_PIN   = 4;
const uint8_t LED3_PIN   = 5;
const uint8_t BUZZER_PIN = 7;

void initialise_les_entrees_sorties() {
   pinMode( BOUTON_PIN, INPUT  );
   pinMode( LED1_PIN  , OUTPUT );
   pinMode( LED2_PIN  , OUTPUT );
   pinMode( LED3_PIN  , OUTPUT );
   pinMode( BUZZER_PIN, OUTPUT );
}

typedef enum {

   ETA_VEILLE,
   ETA_ACTIF,
   ETA_ATTENTE_1,
   ETA_LED_1,
   ETA_ATTENTE_2,
   ETA_LED_2,
   ETA_ATTENTE_3,
   ETA_LED_3,

} Etat_t;

typedef enum {

   EVT_BOUTON,
   EVT_TIMER_3S,
   EVT_TIMER_4S,
   EVT_TIMER_5S,
   EVT_TIMER_6S,
   EVT_TIMER_13S,
   EVT_TIMER_14S,

} Evenement_t;

void stopTone( void );
void demarre();
void eteintTout();
void allume_une_LED();
void allume_deux_LED();
void allume_trois_LED();

static Bouton                                   bouton             ( BOUTON_PIN );
static Automate<Etat_t, Evenement_t>            automate           ( ETA_VEILLE );
static EvenementsTemporels<Etat_t, Evenement_t> evenementsTemporels( automate   );
static ActionsDifferees                         actionsDifferees;

void initialise_l_automate() {
   automate.transition( ETA_VEILLE   , EVT_BOUTON   , ETA_ACTIF    , demarre          );// On appuie sur le bouton pour réveiller le système
   automate.transition( ETA_ACTIF    , EVT_BOUTON   , ETA_VEILLE   , eteintTout       );// Si on appuie, retour à l'état de veille
   automate.transition( ETA_ACTIF    , EVT_TIMER_3S , ETA_ATTENTE_1                   );// Au bout de trois secondes on attend un appui

   automate.transition( ETA_ATTENTE_1, EVT_TIMER_4S , ETA_VEILLE   , eteintTout       );// Si on n'appuie pas, retour à l'état de veille
   automate.transition( ETA_ATTENTE_1, EVT_BOUTON   , ETA_LED_1    , allume_une_LED   );// Si on appuie on allume une LED

   automate.transition( ETA_LED_1    , EVT_BOUTON   , ETA_VEILLE   , eteintTout       );// Si on appuie, retour à l'état de veille
   automate.transition( ETA_LED_1    , EVT_TIMER_5S , ETA_ATTENTE_2                   );// Au bout de cinq secondes on attend un appui

   automate.transition( ETA_ATTENTE_2, EVT_TIMER_6S , ETA_VEILLE   , eteintTout       );// Si on n'appuie pas, retour à l'état de veille
   automate.transition( ETA_ATTENTE_2, EVT_BOUTON   , ETA_LED_2    , allume_deux_LED  );// Si on appuie on allume une seconde LED

   automate.transition( ETA_LED_2    , EVT_BOUTON   , ETA_VEILLE   , eteintTout       );// Si on appuie, retour à l'état de veille
   automate.transition( ETA_LED_2    , EVT_TIMER_13S, ETA_ATTENTE_3                   );// Au bout de cinq secondes on attend un appui

   automate.transition( ETA_ATTENTE_3, EVT_TIMER_14S, ETA_VEILLE   , eteintTout       );// Si on n'appuie pas, retour à l'état de veille
   automate.transition( ETA_ATTENTE_3, EVT_BOUTON   , ETA_LED_3    , allume_trois_LED );// C'est gagné !

   automate.transition( ETA_LED_3    , EVT_BOUTON   , ETA_VEILLE   , eteintTout       );// Pour acquitter la prise d'indice
//   automate.debug();
}

void initialise_les_evenements_temporels() {
   evenementsTemporels.ajoute(  3000, EVT_TIMER_3S  );
   evenementsTemporels.ajoute(  4000, EVT_TIMER_4S  );
   evenementsTemporels.ajoute(  5000, EVT_TIMER_5S  );
   evenementsTemporels.ajoute(  6000, EVT_TIMER_6S  );
   evenementsTemporels.ajoute( 13000, EVT_TIMER_13S );
   evenementsTemporels.ajoute( 14000, EVT_TIMER_14S );
//   evenementsTemporels.debug();
}

void setup() {
   initialise_les_entrees_sorties();
   initialise_la_liaison_serie();
   initialise_l_automate();
   initialise_les_evenements_temporels();
}

void loop() {
   actionsDifferees.evalue();
   evenementsTemporels.evalue();
   if( bouton.est_relache()) {
      automate.evenement( EVT_BOUTON );
   }
}

void stopTone( void ) {
   noTone( BUZZER_PIN );
}

void demarre() {
   Serial.println( "demarre" );
   Serial.println( "Il faut appuyer sur le bouton juste après l'affichage des événements :" );
   Serial.print  ( "   passage dans l'état "); Serial.println( ETA_ATTENTE_1 );
   Serial.print  ( "   passage dans l'état "); Serial.println( ETA_ATTENTE_2 );
   Serial.print  ( "   passage dans l'état "); Serial.println( ETA_ATTENTE_3 );
   evenementsTemporels.arme();
   tone( BUZZER_PIN, 440 );
   actionsDifferees.ajoute( stopTone, 250UL );
}

void eteintTout() {
   Serial.println("eteintTout");
   digitalWrite( LED1_PIN, LOW );
   digitalWrite( LED2_PIN, LOW );
   digitalWrite( LED3_PIN, LOW );
   evenementsTemporels.desactive();
}

void allume_une_LED() {
   Serial.println("allume_une_LED");
   digitalWrite( LED1_PIN, HIGH );
   digitalWrite( LED2_PIN, LOW );
   digitalWrite( LED3_PIN, LOW );
}

void allume_deux_LED() {
   Serial.println("allume_deux_LED");
   digitalWrite( LED1_PIN, HIGH );
   digitalWrite( LED2_PIN, HIGH );
   digitalWrite( LED3_PIN, LOW );
}

void affiche_l_indice() {
   Serial.println("affiche_l_indice");
}

void allume_trois_LED() {
   Serial.println("allume_trois_LED");
   digitalWrite( LED1_PIN, HIGH );
   digitalWrite( LED2_PIN, HIGH );
   digitalWrite( LED3_PIN, HIGH );
   affiche_l_indice();
}
