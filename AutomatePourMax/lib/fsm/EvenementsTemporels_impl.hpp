#pragma once

#include <Arduino.h>
#include <Automate_impl.hpp>

struct EvenementTemporel;
class EvenementsTemporels_impl {
public:

   EvenementsTemporels_impl( Automate_impl & automate );

   void desactive( void );

   void arme( void );

   bool evalue( void );

   void debug( void ) const;

protected:

   void ajoute_( unsigned long delai, int evenement );

private:

   Automate_impl &     _automate;
   EvenementTemporel * _evenementsTemporels;
   unsigned long       _t0;

private:
   EvenementsTemporels_impl( const EvenementsTemporels_impl & );
   EvenementsTemporels_impl & operator = ( const EvenementsTemporels_impl & );
};
