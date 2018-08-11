#pragma once

#include "Window.h"

#include <string>
#include <vector>
#include <algorithm>
#include <locale>

namespace ncurses {

   class LogPane : public Window {
   public:

      explicit LogPane( int x, int height ) :
         Window( x, 0, 40, height ),
         _hasFocus( false ),
         _first( 0 ),
         _autoScroll( true ),
         _selected( 1 )
      {}

   public:

      void render() {
         int count = std::min((int)_lines.size(), getHeight() - 2 );
         for( int i = _first, y = 1; i < count; ++i, ++y ) {
            std::string line = _lines[i].c_str();
            if( line[line.length()-1] == '\n' ) {
               line.erase( line.length() - 1 );
            }
            ::mvwprintw( w(), y, 2, "%s", line.c_str());
         }
         ::wrefresh( w());
      }

      void print( const std::string & line ) {
         if( _lines.empty()) {
            _lines.push_back( trim( line ));
         }
         else {
            std::string & lastLine = _lines[_lines.size()-1];
            if( lastLine.empty() || lastLine[lastLine.length()-1] != '\n' ) {
               lastLine += trim( line );
            }
            else {
               _lines.push_back( trim( line ));
            }
         }
         if( _autoScroll ) {
            unsigned height = w()->_maxy - w()->_begy + 1;
            if( _lines.size() > height ) {
               _first = _lines.size() - height;
            }
         }
         render();
      }

      void println( const std::string & line ) {
         print( trim( line ) + '\n' );
      }

      bool contains( int x, int y ) {
         return ::wmouse_trafo( w(), &y, &x, false );
      }

      void setFocus( bool focus ) {
         _hasFocus = focus;
         ::wmove( w(), _selected, 2 );
      }

      bool hasFocus() const {
         return _hasFocus;
      }

      bool keyPressed( int c ) {
         switch( c ) {
         case KEY_HOME: home(); break;
         case KEY_UP  : up(); break;
         case KEY_DOWN: down(); break;
         case KEY_END : end(); break;
         default: return true;
         }
         return false;
      }

      bool onMouseEvent( MEVENT mouseEvent ) {
         if( ::wmouse_trafo( w(), &mouseEvent.y, &mouseEvent.x, false )) {
            setFocus( true );
            return false;
         }
         return true;
      }

   private:

      void home() {
         _first    = 0;
         _selected = 1;
      }

      void up() {
         if( _selected > 1 ) {
            --_selected;
            --_first;
         }
      }

      void down() {
         if( _first + getHeight() - 2 < _lines.size()) {
            ++_selected;
            ++_first;
         }
      }

      void end() {
         if((int)_lines.size() < getHeight() - 2) {
            _first    = 0;
            _selected = _lines.size() - 1;
         }
         else {
            _selected = getHeight() - 3;
            _first    = _lines.size() - _selected;
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
      unsigned                 _first;
      bool                     _autoScroll;
      unsigned                 _selected;
   };
}
