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
         _haveFocus( false ),
         _label( label )
      {
         ::wmove( w(), y, x );
         ::wprintw( w(), label.c_str());
      }

      virtual ~ Control() {}

   public:

      virtual void render() const  = 0;

      virtual int getXFocus() const = 0;

      virtual bool keyPressed( int c ) = 0;

      virtual void onMouseEvent( const MEVENT & mouseEvent ) = 0;

      virtual bool isHitPoint( int x, int y ) = 0;

   public:

      WINDOW * w() const {
         return _window.w();
      }

      bool haveFocus() const {
         return _haveFocus;
      }

      void setFocus( bool focus ) {
         _haveFocus = focus;
         render();
         ::wmove( _window.w(), _y, getXFocus());
         ::wrefresh( _window.w());
      }

      const std::string & getLabel() const {
         return _label;
      }

   protected:

      Window &    _window;
      int         _x;
      int         _y;
      bool        _haveFocus;
      std::string _label;

   private:
      Control( const Control & );
      Control & operator = ( const Control & );
   };
}
