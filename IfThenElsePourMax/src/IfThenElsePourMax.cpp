#include <Arduino.h>
#include <Servo.h>

/**
 * Etats du système.
 */
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

//--------------------
// Broches connectées
//--------------------
static const uint8_t BOUTON_PIN = 2; //!< Bouton branché sur le port n°2
static const uint8_t LED1_PIN   = 3; //!< DEL branchée sur le port n°3
static const uint8_t LED2_PIN   = 4; //!< DEL branchée sur le port n°4
static const uint8_t LED3_PIN   = 5; //!< DEL branchée sur le port n°5
static const uint8_t MOTEUR_PIN = 6; //!< Moteur branché sur le port n°6 (PWM)
static const uint8_t BUZZER_PIN = 7; //!< Buzzer branché sur le port n°7

//-----------------------------------
// Constantes de réglages du système
//-----------------------------------
static const unsigned long FREQUENCE_DU_BUZZER            =   220UL; //!< Hz Note "La" octave 3
static const unsigned long DUREE_DU_BUZZER                =   160UL; //!< ms Durée du bip
static const unsigned long PERIODE_D_ACTIVATION_DU_BUZZER =   500UL; //!< ms Intervalle entre deux bips
static const unsigned long DEBUT_DE_FENETRE_1             =  3000UL; //!< s  On doit presser le bouton
static const unsigned long FIN_DE_FENETRE_1               =  5000UL; //!< s  On doit attendre
static const unsigned long DEBUT_DE_FENETRE_2             =  7000UL; //!< s  On doit presser le bouton
static const unsigned long FIN_DE_FENETRE_2               =  9000UL; //!< s  On doit attendre
static const unsigned long DEBUT_DE_FENETRE_3             = 12000UL; //!< s  On doit presser le bouton
static const unsigned long FIN_DE_FENETRE_3               = 14000UL; //!< s  On doit attendre

//----------------------
// Variables du système
//----------------------
static Etat_t        etat          = ETA_VEILLE; //!< Etat du système.
static bool          enfonce       = false;      //!< Le bouton a été perçu enfoncé.
static unsigned long t0            = 0UL;        //!< Horodatage des événements : temps de départ.
static unsigned long buzzer_t0     = 0UL;        //!< Horodatage du déclenchement du buzzer.
static unsigned long buzzer_cycle  = 0UL;        //!< N° de la période du buzzer.
static Servo         temps_restant;

/**
 * Eteint le buzzer.
 */
static void desactive_le_buzzer() {
   // Serial.println( "desactive_le_buzzer" );
   noTone( BUZZER_PIN );
}

/**
 * Allume le buzzer et effectue l'horodatage pour gérer son extinction prochaine.
 * @see DUREE_DU_BUZZER
 */
static void active_le_buzzer() {
   // Serial.println( "active_le_buzzer" );
   tone( BUZZER_PIN, FREQUENCE_DU_BUZZER );
   buzzer_t0 = millis();
}

/**
 * Effectue l'horodatage pour gérer les événements temporels.
 * @see DUREE_DU_BUZZER
 * Met en oeuvre l'API Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/functions/time/millis/"
 * >millis()</a>
 */
static void demarre() {
   Serial.println( "demarre" );
   t0 = millis();
}

/**
 * Remet le système en attente, éteint les DEL, le buzzer, réinitialise l'état
 * interne.
 * @see etat, Etat_t.
 */
static void eteint_tout() {
   Serial.println( "eteint_tout" );
   etat          = ETA_VEILLE;
   enfonce       = false;
   t0            = 0;
   buzzer_t0     = 0UL;
   buzzer_cycle  = 0UL;
   desactive_le_buzzer();
   digitalWrite( LED1_PIN, LOW );
   digitalWrite( LED2_PIN, LOW );
   digitalWrite( LED3_PIN, LOW );
   temps_restant.write( 0 );
}

/**
 * Utilise l'API Arduino pour allumer la DEL n°1.
 * Met en oeuvre l'API Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/"
 * >digitalWrite()</a>
 */
static void allume_une_LED() {
   Serial.println( "allume_une_LED" );
   digitalWrite( LED1_PIN, HIGH );
   digitalWrite( LED2_PIN, LOW );
   digitalWrite( LED3_PIN, LOW );
   temps_restant.write( 0 );
}

/**
 * Utilise l'API Arduino pour allumer les DEL n°1 et n°2.
 * Met en oeuvre l'API Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/"
 * >digitalWrite()</a>
 */
static void allume_deux_LED() {
   Serial.println( "allume_deux_LED" );
   digitalWrite( LED1_PIN, HIGH );
   digitalWrite( LED2_PIN, HIGH );
   digitalWrite( LED3_PIN, LOW );
   temps_restant.write( 0 );
}

/**
 * Cette fonction devra, à terme, contrôler un afficheur pour
 * diffuser un message constituant un indice pour résoudre la quête.
 */
static void affiche_l_indice() {
   Serial.println( "affiche_l_indice" );
}

/**
 * Utilise l'API Arduino pour allumer les DEL n°1, n°2 et n°3.
 * Met en oeuvre l'API Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/functions/digital-io/digitalwrite/"
 * >digitalWrite()</a>
 */
static void allume_trois_LED() {
   Serial.println( "allume_trois_LED" );
   digitalWrite( LED1_PIN, HIGH );
   digitalWrite( LED2_PIN, HIGH );
   digitalWrite( LED3_PIN, HIGH );
   temps_restant.write( 0 );
   affiche_l_indice();
}

/**
 * Retourne 'vrai' quand le bouton a été enfoncé puis relâché.
 * Met en oeuvre l'API Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/functions/digital-io/digitalread/"
 * >digitalRead()</a> pour acquérir l'état du bouton.
 * @return true quand le bouton a été enfoncé puis relâché.
 */
static bool bouton_est_relache() {
   if( digitalRead( BOUTON_PIN )) {
      enfonce = true;
   }
   else if( enfonce ) {
      enfonce = false;
      return true;
   }
   return false;
}

/**
 * Utilise le servo moteur pour montrer qu'il faut presser le bouton.
 */
static void montre_le_temps_restant(
   unsigned long temps_ecoule,
   unsigned long debut,
   unsigned long fin )
{
   temps_restant.write( map( temps_ecoule, debut, fin, 0, 179 ));
}

/**
 * Initialise la liaison série au moyen de l'API Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/functions/communication/serial/begin/"
 * >Serial.begin()</a>.
 */
static void initialise_la_liaison_serie() {
   Serial.begin( 115200 );
}

/**
 * Configure les entrées/sorties au moyen de l'API Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/"
 * >pinMode()</a>.
 */
static void initialise_les_entrees_sorties() {
   pinMode( BOUTON_PIN, INPUT  );
   pinMode( LED1_PIN  , OUTPUT );
   pinMode( LED2_PIN  , OUTPUT );
   pinMode( LED3_PIN  , OUTPUT );
   temps_restant.attach( MOTEUR_PIN );
   pinMode( BUZZER_PIN, OUTPUT );
}

/**
 * Initialise le hardware.
 * Voir aussi la documentation Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/structure/sketch/setup/"
 * >setup()</a>.
 */
void setup() {
   initialise_la_liaison_serie();
   Serial.println( "setup" );
   initialise_les_entrees_sorties();
}

/**
 * L'évolution de l'état du système est confiné ici sous la forme d'un automate
 * état/transition.
 * La gestion du bip périodique est également effectuée ici.
 * Voir aussi la documentation Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/structure/sketch/loop/"
 * >loop()</a>.
 */
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
   case ETA_VEILLE:
      if( bouton_est_relache()) {
         demarre();
         etat = ETA_ACTIF;
      }
      break;
   case ETA_ACTIF:
      if( bouton_est_relache()) {
         eteint_tout();
         etat = ETA_VEILLE;
      }
      else if( temps_ecoule >= DEBUT_DE_FENETRE_1 ) {
         etat = ETA_ATTENTE_1;
         Serial.println( "Appui attendu" );
      }
      break;
   case ETA_ATTENTE_1:
      if( bouton_est_relache()) {
         etat = ETA_LED_1;
         allume_une_LED();
      }
      else if( temps_ecoule >= FIN_DE_FENETRE_1 ) {
         eteint_tout();
         etat = ETA_VEILLE;
      }
      else {
         montre_le_temps_restant( temps_ecoule, DEBUT_DE_FENETRE_1, FIN_DE_FENETRE_1 );
      }
      break;
   case ETA_LED_1:
      if( bouton_est_relache()) {
         eteint_tout();
         etat = ETA_VEILLE;
      }
      else if( temps_ecoule >= DEBUT_DE_FENETRE_2 ) {
         etat = ETA_ATTENTE_2;
         Serial.println( "Appui attendu" );
      }
      break;
   case ETA_ATTENTE_2:
      if( bouton_est_relache()) {
         etat = ETA_LED_2;
         allume_deux_LED();
      }
      else if( temps_ecoule >= FIN_DE_FENETRE_2 ) {
         eteint_tout();
         etat = ETA_VEILLE;
      }
      else {
         montre_le_temps_restant( temps_ecoule, DEBUT_DE_FENETRE_2, FIN_DE_FENETRE_2 );
      }
      break;
   case ETA_LED_2:
      if( bouton_est_relache()) {
         eteint_tout();
         etat = ETA_VEILLE;
      }
      else if( temps_ecoule >= DEBUT_DE_FENETRE_3 ) {
         etat = ETA_ATTENTE_3;
         Serial.println( "Appui attendu" );
      }
      break;
   case ETA_ATTENTE_3:
      if( bouton_est_relache()) {
         etat = ETA_LED_3;
         allume_trois_LED();
      }
      else if( temps_ecoule >= FIN_DE_FENETRE_3 ) {
         eteint_tout();
         etat = ETA_VEILLE;
      }
      else {
         montre_le_temps_restant( temps_ecoule, DEBUT_DE_FENETRE_3, FIN_DE_FENETRE_3 );
      }
      break;
   case ETA_LED_3:
      if( bouton_est_relache()) {
         eteint_tout();
         etat = ETA_VEILLE;
      }
      break;
   }
}
