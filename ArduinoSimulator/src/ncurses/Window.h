#pragma once

#include <ncurses.h>

class Window {
public:

   Window( unsigned x, unsigned y, unsigned width, unsigned height ) :
      _window( ::newwin( height, width, y, x ))
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

   int getX() const {
      return w()->_begx;
   }

   int getY() const {
      return w()->_begy;
   }

   int getWidth() const {
      return w()->_maxx + 1;
   }

   int getHeight() const {
      return w()->_maxy + 1;
   }

protected:

   WINDOW * _window;

private:
   Window( const Window & );
   Window & operator = ( const Window & );
};
