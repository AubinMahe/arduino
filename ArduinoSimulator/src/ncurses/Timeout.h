#pragma once

#include "Control.h"
#include "IChangeListener.h"

#include <vector>

namespace ncurses {

   class Timeout : public Control {
   public:

      Timeout( Window & window, int x, int y, const std::string & label, unsigned value ) :
         Control( window, x, y, label ),
         _value    ( value ),
         _remaining( value )
      {}

   public:

      virtual void render() const {
         ::mvwprintw( w(), _y, _x, "%s OUT %3d", _label.c_str(), _remaining );
      }

      virtual bool isFocusable( void ) const {
         return false;
      }

      virtual int getXFocus() const {
         return _x + _label.length() + 1;
      }

      virtual bool keyPressed( int ) {
         return true;
      }

      virtual bool isHitPoint( int, int ) {
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
