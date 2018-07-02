#pragma once

struct Automate_Transition;
class Automate_impl {
public:

   typedef void ( * Action_t )( void );

protected:

   Automate_impl( int etat_courant );

   void transition_( int courant, int evenement, int futur, Action_t action );

public:

   bool evenement_( int evenement );

   void debug() const;

private:

   int                   _etat_courant;
   Automate_Transition * _transitions;

   Automate_impl( const Automate_impl & );
   Automate_impl & operator = ( const Automate_impl & );
};
