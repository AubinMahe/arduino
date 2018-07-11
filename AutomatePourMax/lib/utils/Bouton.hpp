#pragma once

namespace utils {

  class Bouton {
  public:

     Bouton( int port );

     bool est_relache( void );

  private:

     int           _port;
     bool          _est_presse;
     unsigned long _date_de_l_appui;
  };
}
