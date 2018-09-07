#pragma once

#include "Window.h"

#undef min
#undef max
#include <string>
#include <vector>
#include <algorithm>
#include <locale>

namespace ncurses {

   class LogPane : public Window {
   public:

      explicit LogPane( int x, int width, int height ) :
         Window( x, 0, width, height, ACS_TTEE, ACS_URCORNER, ACS_BTEE, ACS_RTEE),
         _hasFocus( false ),
         _scrollable( ::derwin( w(), height - 2, width - 4, 1, 2 )),
         _first( 0 ),
         _autoScroll( true )
      {
         ::scrollok( _scrollable, true );
      }

   public:

      virtual void render( bool clear ) {
         Window::render( clear );
         for( unsigned i = _first, y = 0; i < _lines.size() && y < getHeight() - 2; ++i, ++y ) {
            ::mvwprintw( _scrollable, (int)y, 0, "%s", removeEOL( _lines[i] ).c_str());
         }
         ::wrefresh( _scrollable );
         ::wrefresh( w());
      }

      void print( const std::string & line ) {
         if( _lines.empty()) {
            _lines.push_back( trim( line ));
         }
         else {
            std::string & lastLine = _lines[_lines.size() - 1];
            if( lastLine.empty() || lastLine[lastLine.length()-1] != '\n' ) {
               lastLine += trim( line );
            }
            else {
               _lines.push_back( trim( line ));
            }
         }
         if( _autoScroll && _lines.size() > getHeight() - 2 ) {
            down();
         }
         if( _lines.size() < _first + getHeight() - 1 ) {
            render( false );
         }
      }

      void setFocus( bool focus ) {
         _hasFocus = focus;
         ::wmove( w(), 1, 1 );
      }

      bool hasFocus() const {
         return _hasFocus;
      }

      bool keyPressed( int key ) {
         switch( key ) {
         case KEY_HOME: home(); break;
         case KEY_UP  : up(); break;
         case KEY_DOWN: down(); break;
         case KEY_END : end(); break;
         default: return true;
         }
         return false;
      }

      bool onMouseEvent( MEVENT mouseEvent ) {
         if( ::wmouse_trafo( _scrollable, &mouseEvent.y, &mouseEvent.x, false )) {
            setFocus( true );
            return false;
         }
         return true;
      }

   private:

      static std::string removeEOL( const std::string & i ) {
         std::string            line = i;
         std::string::size_type last = line.length() - 1;
         if( line[last] == '\n' ) {
            line.erase( last );
         }
         return line;
      }

      void home() {
         if( _first > 0 ) {
            _first = 0;
            ::wclear( _scrollable );
            render( false );
         }
      }

      void up() {
         if( _first > 0 ) {
            --_first;
            ::wscrl( _scrollable, -1 );
            ::mvwprintw( _scrollable, 0, 0, "%s", removeEOL( _lines[_first] ).c_str());
            ::wrefresh( _scrollable );
            ::wrefresh( w());
         }
      }

      void down() {
         if( _first <= _lines.size() - getHeight() + 1 ) {
            ++_first;
            ::wscrl( _scrollable, +1 );
            unsigned y = getHeight() - 3;
            unsigned l = _first + y;
            if( l < _lines.size()) {
               ::mvwprintw( _scrollable, y, 0, "%s", removeEOL( _lines[l] ).c_str());
            }
            ::wrefresh( _scrollable );
            ::wrefresh( w());
            _autoScroll = ( _first + getHeight() - 2 == _lines.size());
         }
      }

      void end() {
         _autoScroll = true;
         if( _lines.size() > getHeight() - 2 ) {
            _first = _lines.size() - getHeight() + 2;
            ::wclear( _scrollable );
            render( false );
         }
      }

      static std::string trim( const std::string & str ) {
         auto s = str;
         size_t first = str.find_first_not_of( ' ' );
         if( first == std::string::npos ) {
            return str;
         }
         size_t last = str.find_last_not_of( ' ' );
         return str.substr( first, ( last - first + 1 ));
      }

   private:

      bool                     _hasFocus;
      std::vector<std::string> _lines;
      WINDOW *                 _scrollable;
      unsigned                 _first;
      bool                     _autoScroll;
   };
}
