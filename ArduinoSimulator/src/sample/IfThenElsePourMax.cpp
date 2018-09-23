#include <Arduino.h>

/**
 * Etats du système.
 */
enum Etat_t {
   ETA_VEILLE,
   ETA_MESURE_DU_PREMIER_APPUI,
   ETA_ATTENTE_1,
   ETA_MESURE_DU_DEUXIEME_APPUI,
   ETA_ATTENTE_2,
   ETA_MESURE_DU_TROISIEME_APPUI,
   ETA_SUCCES,
};

//--------------------
// Broches connectées
//--------------------
static const uint8_t BOUTON_PIN = 2; //!< Bouton branché sur le port n°2
static const uint8_t LED1_PIN   = 3; //!< DEL branchée sur le port n°3
static const uint8_t LED2_PIN   = 4; //!< DEL branchée sur le port n°4
static const uint8_t LED3_PIN   = 5; //!< DEL branchée sur le port n°5

//-----------------------------------
// Constantes de réglages du système
//-----------------------------------
static const unsigned long DUREE_TOLERANCE       = 1000UL; //!< en milliseconde
static const unsigned long DUREE_PREMIER_APPUI   = 2000UL; //!< en milliseconde
static const unsigned long DUREE_DEUXIEME_APPUI  = 2000UL; //!< en milliseconde
static const unsigned long DUREE_TROISIEME_APPUI = 2000UL; //!< en milliseconde

//----------------------
// Variables du système
//----------------------
static          Etat_t        etat           = ETA_VEILLE; //!< Etat du système.
static volatile unsigned long t0             = 0UL;        //!< Horodatage des événements : temps de départ.
static volatile unsigned long temps_d_appui  = 0UL;        //!< mesure du temps de pression sur le bouton
static volatile bool          bouton_presse  = false;      //!< L'état du bouton basculé par la routine d'interruption : le_bouton_a_ete_presse().

/**
 * Effectue l'horodatage pour gérer les événements temporels.
 * @see DUREE_DU_BUZZER
 * Met en oeuvre l'API Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/functions/time/millis/"
 * >millis()</a>
 */
static void demarre() {
   Serial.println( "demarre" );
}

/**
 * Remet le système en attente, éteint les DEL, le buzzer, réinitialise l'état
 * interne.
 * @see etat, Etat_t.
 */
static void eteint_tout() {
   Serial.println( "eteint_tout" );
   etat = ETA_VEILLE;
   t0   = 0UL;
   digitalWrite( LED1_PIN, LOW );
   digitalWrite( LED2_PIN, LOW );
   digitalWrite( LED3_PIN, LOW );
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
   affiche_l_indice();
}

/**
 * Routine d'interruption attachée au front descendant du bouton (pin 2) au
 * moyen de l'API Arduino <a target="arduino-page" href=
 * "https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/"
 * >attachInterrupt()</a>.
 */
static void le_bouton_a_change( void ) {
   if( bouton_presse ) {
      temps_d_appui = millis() - t0;
      bouton_presse = false;
   }
   else {
      t0            = millis();
      bouton_presse = true;
   }
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
 * Enregistre la routine d'interruption le_bouton_a_ete_presse() attachée au
 * front descendant du bouton (pin 2) au moyen de l'API Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/"
 * >attachInterrupt()</a>.
 */
static void initialise_les_entrees_sorties() {
   pinMode( BOUTON_PIN, INPUT  );
   pinMode( LED1_PIN  , OUTPUT );
   pinMode( LED2_PIN  , OUTPUT );
   pinMode( LED3_PIN  , OUTPUT );
   bouton_presse = false;
   attachInterrupt( digitalPinToInterrupt( BOUTON_PIN ), le_bouton_a_change, CHANGE );
}

/**
 * Initialise le hardware.
 * Voir aussi la documentation Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/structure/sketch/setup/"
 * >setup()</a>.
 */
void setup( void ) {
   initialise_la_liaison_serie();
   Serial.println( "setup" );
   initialise_les_entrees_sorties();
}

unsigned long last_tick = 0;

/**
 * L'évolution de l'état du système est confiné ici sous la forme d'un automate
 * état/transition.
 * Voir aussi la documentation Arduino <a target="arduino-page"
 * href="https://www.arduino.cc/reference/en/language/structure/sketch/loop/"
 * >loop()</a>.
 */
void loop( void ) {
   unsigned long temps_ecoule = millis() - t0;
   switch( etat ) {
   case ETA_VEILLE:
      if( bouton_presse ) {
         demarre();
         etat = ETA_MESURE_DU_PREMIER_APPUI;
      }
      break;
   case ETA_MESURE_DU_PREMIER_APPUI:
      if( ! bouton_presse ) {
         if(  ( temps_d_appui > DUREE_PREMIER_APPUI )
            &&( temps_d_appui < ( DUREE_PREMIER_APPUI + DUREE_TOLERANCE )))
         {
            etat = ETA_ATTENTE_1;
            allume_une_LED();
         }
         else {
            eteint_tout();
         }
      }
      else if( temps_ecoule > ( DUREE_PREMIER_APPUI + DUREE_TOLERANCE )) {
         eteint_tout();
      }
      break;
   case ETA_ATTENTE_1:
      if( bouton_presse ) {
         etat = ETA_MESURE_DU_DEUXIEME_APPUI;
      }
      break;
   case ETA_MESURE_DU_DEUXIEME_APPUI:
      if( ! bouton_presse ) {
         if(  ( temps_d_appui > DUREE_DEUXIEME_APPUI )
            &&( temps_d_appui < ( DUREE_DEUXIEME_APPUI + DUREE_TOLERANCE )))
         {
            etat = ETA_ATTENTE_2;
            allume_deux_LED();
         }
         else {
            eteint_tout();
         }
      }
      else if( temps_ecoule > ( DUREE_DEUXIEME_APPUI + DUREE_TOLERANCE )) {
         eteint_tout();
      }
      break;
   case ETA_ATTENTE_2:
      if( bouton_presse ) {
         etat = ETA_MESURE_DU_TROISIEME_APPUI;
      }
      break;
   case ETA_MESURE_DU_TROISIEME_APPUI:
      if( ! bouton_presse ) {
         if(  ( temps_d_appui > DUREE_TROISIEME_APPUI )
            &&( temps_d_appui < ( DUREE_TROISIEME_APPUI + DUREE_TOLERANCE )))
         {
            etat = ETA_SUCCES;
            allume_trois_LED();
         }
         else {
            eteint_tout();
         }
      }
      else if( temps_ecoule > ( DUREE_TROISIEME_APPUI + DUREE_TOLERANCE )) {
         eteint_tout();
      }
      break;
   case ETA_SUCCES:
      if( bouton_presse ) {
         eteint_tout();
      }
      break;
   }
}

void tear_down( void ) {
   Serial.println( "fin du programme simulé." );
}
