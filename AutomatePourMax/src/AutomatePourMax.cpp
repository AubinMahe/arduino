#include <ActionsAsynchrones.hpp>
#include <Bouton.hpp>
#include <EvenementsTemporels.hpp>

#include <Arduino.h>
#ifndef HAVE_HWSERIAL0
extern HardwareSerial Serial;
#endif

namespace max {

   class Jeu : public utils::Acteur {
   public:
      Jeu() : _bouton( BOUTON_PIN ), _automate( ETA_VEILLE ), _evenementsTemporels( _automate ) {
         initialise_les_entrees_sorties();
         initialise_la_liaison_serie();
         initialise_l_automate();
         initialise_les_evenements_temporels();
      }

      void boucle() {
         utils::Acteur& acteur = *this;
         _actionsAsynchrones.evalue( acteur );
         _evenementsTemporels.evalue( acteur );
         if( _bouton.est_relache() ) {
            _automate.evenement( acteur, EVT_BOUTON );
         }
      }

   private:
      static const uint8_t       BOUTON_PIN                     = 2;
      static const uint8_t       LED1_PIN                       = 3;
      static const uint8_t       LED2_PIN                       = 4;
      static const uint8_t       LED3_PIN                       = 5;
      static const uint8_t       BUZZER_PIN                     = 7;
      static const unsigned long FREQUENCE_DU_BUZZER            = 220;
      static const unsigned long DUREE_DU_BUZZER                = 160;
      static const unsigned long PERIODE_D_ACTIVATION_DU_BUZZER = 500UL;

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

      enum Evenement_t {
         EVT_BOUTON,
         EVT_DEBUT_DE_FENETRE_1,
         EVT_FIN_DE_FENETRE_1,
         EVT_DEBUT_DE_FENETRE_2,
         EVT_FIN_DE_FENETRE_2,
         EVT_DEBUT_DE_FENETRE_3,
         EVT_FIN_DE_FENETRE_3,
      };

      void initialise_la_liaison_serie() {
         Serial.begin( 115200 );
         Serial.println( "setup" );
      }

      void initialise_les_entrees_sorties() {
         pinMode( BOUTON_PIN, INPUT );
         pinMode( LED1_PIN, OUTPUT );
         pinMode( LED2_PIN, OUTPUT );
         pinMode( LED3_PIN, OUTPUT );
         pinMode( BUZZER_PIN, OUTPUT );
      }

      void initialise_l_automate() {
         _automate.transition(
            ETA_VEILLE, EVT_BOUTON, ETA_ACTIF,
            (utils::action_t)&Jeu::demarre ); // On appuie sur le bouton pour réveiller le système
         _automate.transition(
            ETA_ACTIF, EVT_BOUTON, ETA_VEILLE,
            (utils::action_t)&Jeu::eteintTout ); // Si on appuie, retour à l'état de veille
         _automate.transition( ETA_ACTIF, EVT_DEBUT_DE_FENETRE_1,
                               ETA_ATTENTE_1 ); // On attend un appui

         _automate.transition(
            ETA_ATTENTE_1, EVT_FIN_DE_FENETRE_1, ETA_VEILLE,
            (utils::action_t)&Jeu::eteintTout ); // Si on n'appuie pas, retour à l'état de veille
         _automate.transition(
            ETA_ATTENTE_1, EVT_BOUTON, ETA_LED_1,
            (utils::action_t)&Jeu::allume_une_LED ); // Si on appuie on allume une LED&Jeu::

         _automate.transition(
            ETA_LED_1, EVT_BOUTON, ETA_VEILLE,
            (utils::action_t)&Jeu::eteintTout ); // Si on appuie, retour à l'état de veille
         _automate.transition( ETA_LED_1, EVT_DEBUT_DE_FENETRE_2,
                               ETA_ATTENTE_2 ); // On attend un appui

         _automate.transition(
            ETA_ATTENTE_2, EVT_FIN_DE_FENETRE_2, ETA_VEILLE,
            (utils::action_t)&Jeu::eteintTout ); // Si on n'appuie pas, retour à l'état de veille
         _automate.transition(
            ETA_ATTENTE_2, EVT_BOUTON, ETA_LED_2,
            (utils::action_t)&Jeu::allume_deux_LED ); // Si on appuie on allume une seconde LED

         _automate.transition(
            ETA_LED_2, EVT_BOUTON, ETA_VEILLE,
            (utils::action_t)&Jeu::eteintTout ); // Si on appuie, retour à l'état de veille
         _automate.transition( ETA_LED_2, EVT_DEBUT_DE_FENETRE_3,
                               ETA_ATTENTE_3 ); // On attend un appui

         _automate.transition(
            ETA_ATTENTE_3, EVT_FIN_DE_FENETRE_3, ETA_VEILLE,
            (utils::action_t)&Jeu::eteintTout ); // Si on n'appuie pas, retour à l'état de veille
         _automate.transition( ETA_ATTENTE_3, EVT_BOUTON, ETA_LED_3,
                               (utils::action_t)&Jeu::allume_trois_LED ); // C'est gagné !

         _automate.transition(
            ETA_LED_3, EVT_BOUTON, ETA_VEILLE,
            (utils::action_t)&Jeu::eteintTout ); // Pour acquitter la prise d'indice
         //_automate.debug();
      }

      void initialise_les_evenements_temporels() {
         _evenementsTemporels.ajoute( 3000, EVT_DEBUT_DE_FENETRE_1 );
         _evenementsTemporels.ajoute( 5000, EVT_FIN_DE_FENETRE_1 );
         _evenementsTemporels.ajoute( 7000, EVT_DEBUT_DE_FENETRE_2 );
         _evenementsTemporels.ajoute( 9000, EVT_FIN_DE_FENETRE_2 );
         _evenementsTemporels.ajoute( 12000, EVT_DEBUT_DE_FENETRE_3 );
         _evenementsTemporels.ajoute( 14000, EVT_FIN_DE_FENETRE_3 );
         //_evenementsTemporels.debug();
      }

      void desactive_le_buzzer() {
         // Serial.println( "desactive_le_buzzer" );
         noTone( BUZZER_PIN );
      }

      void active_le_buzzer() {
         // Serial.println( "active_le_buzzer" );
         tone( BUZZER_PIN, FREQUENCE_DU_BUZZER );
         _actionsAsynchrones.differee( (utils::action_t)&Jeu::desactive_le_buzzer, DUREE_DU_BUZZER );
      }

      void demarre() {
         Serial.println(
            "demarre. Il faut appuyer sur le bouton juste après le passage dans l'état 2, 4 ou 6" );
         _actionsAsynchrones.periodique( (utils::action_t)&Jeu::active_le_buzzer,
                                         PERIODE_D_ACTIVATION_DU_BUZZER );
         _evenementsTemporels.arme();
         //_evenementsTemporels.debug();
         //_automate.debug();
      }

      void eteintTout() {
         Serial.println( "eteintTout" );
         _evenementsTemporels.desarme();
         _actionsAsynchrones.desactive( (utils::action_t)&Jeu::active_le_buzzer );
         desactive_le_buzzer();
         digitalWrite( LED1_PIN, LOW );
         digitalWrite( LED2_PIN, LOW );
         digitalWrite( LED3_PIN, LOW );
      }

      void allume_une_LED() {
         Serial.println( "allume_une_LED" );
         digitalWrite( LED1_PIN, HIGH );
         digitalWrite( LED2_PIN, LOW );
         digitalWrite( LED3_PIN, LOW );
      }

      void allume_deux_LED() {
         Serial.println( "allume_deux_LED" );
         digitalWrite( LED1_PIN, HIGH );
         digitalWrite( LED2_PIN, HIGH );
         digitalWrite( LED3_PIN, LOW );
      }

      void affiche_l_indice() { Serial.println( "affiche_l_indice" ); }

      void allume_trois_LED() {
         Serial.println( "allume_trois_LED" );
         digitalWrite( LED1_PIN, HIGH );
         digitalWrite( LED2_PIN, HIGH );
         digitalWrite( LED3_PIN, HIGH );
         affiche_l_indice();
      }

   private:
      utils::Bouton                                   _bouton;
      fsm::Automate< Etat_t, Evenement_t >            _automate;
      fsm::EvenementsTemporels< Etat_t, Evenement_t > _evenementsTemporels;
      utils::ActionsAsynchrones                       _actionsAsynchrones;
   };
} // namespace max

static max::Jeu* jeu = 0;

void setup() {
   jeu = new max::Jeu();
}

void loop() {
   jeu->boucle();
}
