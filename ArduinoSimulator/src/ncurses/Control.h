#pragma once

#include "Window.h"

#include <string>

namespace ncurses {

   class Control {
   public:

      Control( Window & window, int x, int y, const std::string & label ) :
         _window( window ),
         _x( x ),
         _y( y ),
         _hasFocus( false ),
         _label( label )
      {}

      virtual ~ Control( void ) {}

   public:

      virtual void render( void ) const  = 0;

      virtual bool isFocusable( void ) const = 0;

      virtual int getXFocus( void ) const = 0;

      virtual bool keyPressed( int c ) = 0;

      virtual void onMouseEvent( const MEVENT & mouseEvent ) = 0;

      virtual bool isHitPoint( int x, int y ) = 0;

   public:

      WINDOW * w( void ) const {
         return _window.w();
      }

      bool hasFocus( void ) const {
         return _hasFocus;
      }

      void setFocus( bool focus ) {
         _hasFocus = focus;
         render();
         ::wmove( w(), _y, getXFocus());
      }

      const std::string & getLabel( void ) const {
         return _label;
      }

   protected:

      Window &    _window;
      int         _x;
      int         _y;
      bool        _hasFocus;
      std::string _label;

   private:
      Control( const Control & );
      Control & operator = ( const Control & );
   };
}
