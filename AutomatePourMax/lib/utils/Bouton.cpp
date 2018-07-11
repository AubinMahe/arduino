#include "Bouton.hpp"

#include <Arduino.h> // millis(), digitalRead()

unsigned long millis(void);

using namespace utils;

static const unsigned int DUREE_MINIMALE_D_UN_APPUI  = 40; // ms

Bouton::Bouton( int port ) {
   _port            = port;
   _est_presse      = false;
   _date_de_l_appui = 0UL;
}

bool Bouton::est_relache( void ) {
   unsigned long maintenant = millis();
   if( digitalRead( _port )) {
      if( ! _est_presse ) {
         _date_de_l_appui = maintenant;
      }
      _est_presse = true;
   }
   else if( _est_presse ) {
      unsigned long duree_de_l_appui = ( maintenant - _date_de_l_appui );
      if( duree_de_l_appui > DUREE_MINIMALE_D_UN_APPUI ) {
         _est_presse      = false;
         _date_de_l_appui = 0;
         return true;
      }
   }
   return false;
}
