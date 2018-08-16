#pragma once

#include "Control.h"
#include "utf8len.h"

namespace ncurses {

   class Slider : public Control {
   public:

      Slider(
         Window &             window,
         unsigned             x,
         unsigned             y,
         unsigned             width,
         const std::wstring & label,
         int                  min,
         int                  max )
       :
         Control( window, x, y, label ),
         _width( width ),
         _min( min ),
         _max( max ),
         _value( min ),
         _step((unsigned)ceil(( max - min ) /(double)( width - 1.0 ))),
         _left( x + _label.length() + 1 ),
         _right( _left + _width )
      {}

   public:

      virtual void render() const {
         ::wmove    ( w(), _y, _x );
         waddwstr   ( w(), _label.c_str());
         ::mvwprintw( w(), _y, _left - 1, "[" );
         ::mvwprintw( w(), _y, _right, "] %4d", _value );
         for( auto i = 0U; i < _width; ++i ) {
            auto x = _left + i;
            ::wmove( w(), _y, x );
            if( x == getXFocus()) {
               ::waddch( w(), ACS_PLUS | A_BOLD );
            }
            else {
               ::waddch( w(), ACS_HLINE );
            }
         }
      }

      virtual bool isFocusable( void ) const {
         return true;
      }

      virtual unsigned getXFocus() const {
         double pos = _left + _width * ( _value - _min ) / ( _max - _min );
         if( pos > _left + _width - 1 ) {
            pos = _left + _width - 1;
         }
         return pos;
      }

      virtual bool keyPressed( int key ) {
         switch( key ) {
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

      virtual bool isHitPoint( unsigned x, unsigned y ) {
         return ( y == _y )&&( x >= _left )&&( x < _right );
      }

      virtual void onMouseEvent( const MEVENT & mouseEvent ) {
         _value = ( mouseEvent.x - _left) * _step;
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

      unsigned _width;
      int      _min;
      int      _max;
      int      _value;
      unsigned _step;
      unsigned _left;
      unsigned _right;
   };
}
