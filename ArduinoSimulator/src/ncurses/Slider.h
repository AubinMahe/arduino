#pragma once

#include "Control.h"

namespace ncurses {

   class Slider : public Control {
   public:

      Slider( Window & window, int x, int y, int width, const std::string & label, int min, int max ) :
         Control( window, x, y, label ),
         _width( width ),
         _min( min ),
         _max( max ),
         _value( min ),
         _step(( max - min ) / width ),
         _line( _width, '-' )
      {}

   public:

      virtual void render() const {
         ::mvwprintw( w(), _y, _x, "%s [%s] %4d", _label.c_str(), _line.c_str(), _value );
         ::wmove    ( w(), _y, getXFocus());
         ::waddch   ( w(), '|' | A_BOLD );
      }

      virtual bool isFocusable( void ) const {
         return true;
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
         return _x + _label.length() + 2;
      }

   protected:

      int         _width;
      int         _min;
      int         _max;
      int         _value;
      int         _step;
      std::string _line;
   };
}
