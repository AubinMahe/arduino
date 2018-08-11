#pragma once

#include "Control.h"

namespace ncurses {

   class Slider : public Control {
   public:

      Slider( Window & window, int x, int y, int width, const std::string & label, int min, int max ) :
         Control( window, x, y, label + " [" + std::string( width, '-' ) + ']' ),
         _width( width ),
         _min( min ),
         _max( max ),
         _value( min ),
         _step(( max - min ) / width )
      {
         ::mvwprintw( w(), y, x, _label.c_str());
         ::wrefresh ( w());
         render();
      }

   public:

      virtual void render() const {
         ::wmove( w(), _y, left());
         for( auto i = 0; i < _width; ++i ) {
            ::waddch( w(), '-' );
         }
         ::wmove   ( w(), _y, getXFocus());
         ::waddch  ( w(), '|' | A_BOLD );
         ::wmove   ( w(), _y, getXFocus());
         ::wrefresh( w());
      }

      virtual int getXFocus() const {
         double pos = round( left() + _width * ( _value - _min ) / (double)( _max - _min + 1.0 ));
         if( pos > left() + _width - 1 ) {
            pos = left() + _width - 1;
         }
         return pos;
      }

      virtual bool keyPressed( int c ) {
         switch( c ) {
         case KEY_PPAGE: _value = _min; break;
         case KEY_LEFT :
            if( _value > _min ) {
               _value -= _step;
               if( _value < _min ) {
                  _value = _min;
               }
            }
         break;
         case KEY_RIGHT:
            if( _value < _max ) {
               _value += _step;
               if( _value > _max ) {
                  _value = _max;
               }
            }
         break;
         case KEY_NPAGE: _value = _max; break;
         default: return true;
         }
         render();
         return false;
      }

      virtual bool isHitPoint( int x, int y ) {
         return ( y == _y )&&( x >= left())&&( x < (int)_label.length());
      }

      virtual void onMouseEvent( const MEVENT & mouseEvent ) {
         _value = ( mouseEvent.x - left() + 1 ) * _step;
         if( _value < _min ) {
            _value = _min;
         }
         else if( _value > _max ) {
            _value = _max;
         }
         render();
      }

      int getValue() const {
         return _value;
      }

      void setValue( int value ) {
         _value = value;
      }

   protected:

      int left() const {
         return _x + _label.length() - _width - 2;
      }

   protected:

      int _width;
      int _min;
      int _max;
      int _value;
      int _step;
   };
}
