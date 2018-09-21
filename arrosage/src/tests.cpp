#ifdef HPMS_TESTS

#include <IUI.h>

static int testActivite() {
   // 1: {{ 8,  0 }, 20 }, {{ 22,  0 }, 20 }
   // 2: {{ 8, 15 }, 20 }, {{ 22, 15 }, 20 }
   // 3: {{ 8, 30 }, 20 }, {{ 22, 30 }, 20 }
   // 4: {{ 8, 45 }, 20 }, {{ 22, 45 }, 20 }

   hpms::Arrosage a;
   hpms::Horloge  h;
   a.demarrer( true );
   h.actualiser(  7, 45 ); a.evaluer( h ); // Rien
   h.actualiser(  8, 01 ); a.evaluer( h ); // Ouverture de la vanne N°1
   h.actualiser(  8, 16 ); a.evaluer( h ); // Ouverture de la vanne N°2
   h.actualiser(  8, 21 ); a.evaluer( h ); // Fermeture de la vanne N°1
   h.actualiser(  8, 31 ); a.evaluer( h ); // Ouverture de la vanne N°3
   h.actualiser(  8, 36 ); a.evaluer( h ); // Fermeture de la vanne N°2
   h.actualiser(  8, 46 ); a.evaluer( h ); // Ouverture de la vanne N°4
   h.actualiser(  8, 51 ); a.evaluer( h ); // Fermeture de la vanne N°3
   h.actualiser(  9, 06 ); a.evaluer( h ); // Fermeture de la vanne N°4
   h.actualiser( 21, 59 ); a.evaluer( h ); // Rien
   h.actualiser( 22, 01 ); a.evaluer( h ); // Ouverture de la vanne N°1
   h.actualiser( 22, 16 ); a.evaluer( h ); // Ouverture de la vanne N°2
   h.actualiser( 22, 21 ); a.evaluer( h ); // Fermeture de la vanne N°1
   h.actualiser( 22, 31 ); a.evaluer( h ); // Ouverture de la vanne N°3
   h.actualiser( 22, 36 ); a.evaluer( h ); // Fermeture de la vanne N°2
   h.actualiser( 22, 46 ); a.evaluer( h ); // Ouverture de la vanne N°4
   h.actualiser( 22, 51 ); a.evaluer( h ); // Fermeture de la vanne N°3
   h.actualiser( 23, 06 ); a.evaluer( h ); // Fermeture de la vanne N°4
   return EXIT_SUCCESS;
}

void executeAutoTestsIfAny() {
   const std::vector<std::string> & args = sim::IUI::_theUI->getArguments();
   for( int i = 0; i < args.size(); ++i ) {
      if( args[i] == "--test=Activite" ) {
         ::exit( testActivite());
      }
   }
}

#endif
