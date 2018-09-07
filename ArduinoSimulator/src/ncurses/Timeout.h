#pragma once

#include "Control.h"
#include "IChangeListener.h"

#undef min
#undef max
#include <vector>

namespace ncurses {

   class Timeout : public Control {
   public:

      Timeout(
         Window &             window,
         unsigned             x,
         unsigned             y,
         const std::wstring & label,
         unsigned             value )
      :
         Control( window, x, y, label ),
         _value    ( value ),
         _remaining( value )
      {}

   public:

      virtual void render() const {
         int x = _label.length() + 2;
         ::wmove    ( w(), _y, _x );
         waddwstr   ( w(), _label.c_str());
         ::mvwprintw( w(), _y, x, " OUT %3d", _remaining );
      }

      virtual bool isFocusable( void ) const {
         return false;
      }

      virtual unsigned getXFocus() const {
         return _x + _label.length() + 1;
      }

      virtual bool keyPressed( int ) {
         return true;
      }

      virtual bool isHitPoint( unsigned, unsigned ) {
         return false;
      }

      virtual void onMouseEvent( const MEVENT & ) {
      }

      void setRemaining( unsigned value ) {
         _remaining = value;
         render();
      }

   protected:

      unsigned _value;
      unsigned _remaining;
   };
}
