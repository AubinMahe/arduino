#pragma once

#include "Window.h"

#include <string>
#include <vector>
#include <algorithm>
#include <cstdarg>

namespace ncurses {

   class StatusPane : public Window {
   public:

      StatusPane( int y, int width ) :
         Window( 0, y, width, 4, ACS_LTEE, ACS_RTEE )
      {}

   public:

      virtual void render( bool clear ) {
         Window::render( clear );
         ::mvwprintw( w(), 1, 2, "%s", _line1.c_str());
         ::mvwprintw( w(), 2, 2, "%s", _line2.c_str());
         ::wrefresh ( w());
      }

      void setLine1( const char * fmt... ) {
         va_list args;
         va_start( args, fmt );
         setLine( _line1, fmt, args );
      }

      void setLine2( const char * fmt... ) {
         va_list args;
         va_start( args, fmt );
         setLine( _line2, fmt, args );
      }

   private:

      void setLine( std::string & target, const char * fmt, va_list args1 ) {
         va_list args2;
         va_copy( args2, args1 );
         std::vector<char> buf( 1 + std::vsnprintf( NULL, 0, fmt, args1 ));
         va_end( args1 );
         std::vsnprintf( buf.data(), buf.size(), fmt, args2 );
         va_end( args2 );
         target = buf.data();
         render( false );
      }

   private:

      std::string _line1;
      std::string _line2;
   };
}
