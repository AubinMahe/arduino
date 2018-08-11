#pragma once

#include "Control.h"
#include "IChangeListener.h"

#include <vector>

namespace ncurses {

   class Checkbox : public Control {
   public:

      Checkbox( Window & window, int x, int y, const std::string & label ) :
         Control( window, x, y, label + " [ ]" ),
         _readOnly( false ),
         _checked ( false )
      {}

   public:

      virtual void render() const {
         ::mvwprintw( w(), _y, _x, _label.c_str());
         ::wmove( w(), _y, getXFocus());
         if( _checked ) {
            ::waddch( w(), 'X' | A_BOLD );
         }
         else {
            ::waddch( w(), ' ' );
         }
         ::wmove( w(), _y, getXFocus());
         ::wrefresh( _window.w());
      }

      virtual int getXFocus() const {
         return _x + _label.length() - 3;
      }

      virtual bool keyPressed( int c ) {
         if( ! _readOnly ) {
            if( c == 32 || c == 13 ) {
               setChecked( ! _checked );
               render();
               return false;
            }
         }
         return true;
      }

      virtual bool isHitPoint( int x, int y ) {
         return ( y == _y )&&( x == getXFocus());
      }

      virtual void onMouseEvent( const MEVENT & ) {
         if( ! _readOnly ) {
            setChecked( ! _checked );
            render();
         }
      }

      bool isChecked( void ) const {
         return _checked;
      }

      void setChecked( bool value ) {
         bool before = _checked;
         _checked = value;
         if( _checked != before ) {
            for( auto it = _listeners.begin(); it != _listeners.end(); ++it ) {
               (*it)->hasChanged( *this, before, _checked );
            }
         }
         render();
      }

      bool isReadOnly( void ) const {
         return _readOnly;
      }

      void setReadOnly( bool value ) {
         _readOnly = value;
      }

      void addChangeListener( IChangeListener<Checkbox, bool> * l ) {
         _listeners.push_back( l );
      }

   protected:

      std::vector<
         IChangeListener<
            Checkbox, bool>*> _listeners;
      bool                    _readOnly;
      bool                    _checked;
   };
}
