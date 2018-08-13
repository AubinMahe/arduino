#pragma once

#include "Window.h"

#include <string>

namespace ncurses {

   class Control {
   public:

      Control( Window & window, unsigned x, unsigned y, const std::wstring & label ) :
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

      virtual unsigned getXFocus( void ) const = 0;

      virtual bool keyPressed( int key ) = 0;

      virtual void onMouseEvent( const MEVENT & mouseEvent ) = 0;

      virtual bool isHitPoint( unsigned x, unsigned y ) = 0;

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

      const std::wstring & getLabel() const {
         return _label;
      }

   protected:

      Window &     _window;
      unsigned     _x;
      unsigned     _y;
      bool         _hasFocus;
      std::wstring _label;

   private:
      Control( const Control & );
      Control & operator = ( const Control & );
   };
}
