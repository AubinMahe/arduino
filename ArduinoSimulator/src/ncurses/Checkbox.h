#pragma once

#include "Control.h"
#include "IChangeListener.h"
#include "utf8len.h"

#undef min
#undef max
#include <vector>
#include <algorithm>

namespace ncurses {

   class Checkbox : public Control {
   public:

      Checkbox( Window & window, int x, int y, const std::wstring & label ) :
         Control( window, x, y, label ),
         _mode( -1 ),
         _checked( false )
      {}

   public:

      virtual void render() const {
         const char * mode = ( _mode == INPUT ) ? "IN " : (( _mode == OUTPUT ) ? "OUT" : "---" );
         int x = _label.length() + 2;
         ::wmove    ( w(), _y, _x );
         waddwstr   ( w(), _label.c_str());
         ::mvwprintw( w(), _y, x, " %s [ ]", mode );
         ::wmove    ( w(), _y, getXFocus());
         ::waddch   ( w(), _checked ? ('X'|A_BOLD) : ' ' );
         ::wmove    ( w(), _y, getXFocus());
      }

      virtual bool isFocusable( void ) const {
         return _mode == INPUT;
      }

      virtual unsigned getXFocus() const {
         return _x + _label.length() + 6;
      }

      virtual bool keyPressed( int key ) {
         if( _mode == INPUT ) {
            if( key == 32 || key == 13 ) {
               setChecked( ! _checked );
               render();
               return false;
            }
         }
         return true;
      }

      virtual bool isHitPoint( unsigned x, unsigned y ) {
         return ( y == _y )&&( x == getXFocus());
      }

      virtual void onMouseEvent( const MEVENT & ) {
         if( _mode == INPUT ) {
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
         return _mode != INPUT;
      }

      void setMode( int mode ) {
         _mode = mode;
         render();
      }

      void addChangeListener( IChangeListener<Checkbox, bool> * l ) {
         if( std::find( _listeners.begin(), _listeners.end(), l ) == _listeners.end()) {
            _listeners.push_back( l );
         }
      }

   protected:

      std::vector<
         IChangeListener<
            Checkbox, bool>*> _listeners;
      int                     _mode;
      bool                    _checked;
   };
}
