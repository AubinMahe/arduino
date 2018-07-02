#pragma once

class ActionDifferee;
class ActionsDifferees {
public:

   ActionsDifferees( void );

   typedef void ( * actionDiffere_t )( void );

   void ajoute( actionDiffere_t _action, unsigned long _delai );

   void evalue( void );

private:

   ActionDifferee * _actions;

private:
   ActionsDifferees( const ActionsDifferees &);
   ActionsDifferees & operator = ( const ActionsDifferees &);
};
