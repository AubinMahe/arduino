#pragma once

#include <ncurses.h>

class Window {
public:

   Window( unsigned x, unsigned y, unsigned width, unsigned height ) :
      _window( ::newwin( height, width, y, x )),
      _height( height - 2 )

   {
      ::box( _window, 0, 0 );
      ::keypad( w(), TRUE );
      ::wrefresh( _window );
   }

   ~ Window() {
      ::wborder ( _window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' );
      ::wrefresh( _window );
      ::delwin  ( _window );
   }

public:

   inline WINDOW * w( void ) const { return _window; }

protected:

   WINDOW * _window;
   unsigned _height;

private:
   Window( const Window & );
   Window & operator = ( const Window & );
};
