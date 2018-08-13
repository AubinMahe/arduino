#pragma once

#include <ncursesw/curses.h>

class Window {
public:

   Window(
      unsigned x,
      unsigned y,
      unsigned width,
      unsigned height,
      chtype   topLeft     = ACS_ULCORNER,
      chtype   topRight    = ACS_URCORNER,
      chtype   bottomLeft  = ACS_LLCORNER,
      chtype   bottomRight = ACS_LRCORNER,
      chtype   leftSide    = ACS_VLINE,
      chtype   rightSide   = ACS_VLINE,
      chtype   topSide     = ACS_HLINE,
      chtype   bottomSide  = ACS_HLINE )
   :
      _width( width ),
      _height( height ),
      _window( ::newwin( _height, _width, y, x )),
      _topLeft    ( topLeft  ),
      _topRight   ( topRight ),
      _bottomLeft ( bottomLeft  ),
      _bottomRight( bottomRight ),
      _leftSide   ( leftSide   ),
      _rightSide  ( rightSide  ),
      _topSide    ( topSide    ),
      _bottomSide ( bottomSide )
   {
      ::keypad ( _window, TRUE );
      ::wborder( _window,
         _leftSide,
         _rightSide,
         _topSide,
         _bottomSide,
         _topLeft,
         _topRight,
         _bottomLeft,
         _bottomRight );
   }

   virtual ~ Window( void ) {
      ::wclear  ( _window );
      ::wborder ( _window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' );
      ::wrefresh( _window );
      ::delwin  ( _window );
   }

public:

   virtual void render( bool clear ) {
      if( clear ) {
         ::wclear ( _window );
         ::wresize( _window, _height, _width );
      }
      ::wborder( _window,
         _leftSide,
         _rightSide,
         _topSide,
         _bottomSide,
         _topLeft,
         _topRight,
         _bottomLeft,
         _bottomRight );
   }

   inline WINDOW * w( void ) const { return _window; }

   unsigned getX() const {
      return _window->_begx;
   }

   unsigned getY() const {
      return _window->_begy;
   }

   unsigned getWidth() const {
      return _width;
   }

   void setWidth( unsigned width ) {
      _width = width;
      ::wresize( _window, _height, _width );
   }

   unsigned getHeight() const {
      return _height;
   }

protected:

   unsigned _width;
   unsigned _height;
   WINDOW * _window;
   chtype   _topLeft;
   chtype   _topRight;
   chtype   _bottomLeft;
   chtype   _bottomRight;
   chtype   _leftSide;
   chtype   _rightSide;
   chtype   _topSide;
   chtype   _bottomSide;

private:
   Window( const Window & );
   Window & operator = ( const Window & );
};
