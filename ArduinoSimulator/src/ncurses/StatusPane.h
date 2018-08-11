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
         Window( 0, y, width, 4 )
      {
         fprintf( stderr, "StatusPane( int y = %d, int width = %d )\n", y, width );
      }

   public:

      void render() {
         ::mvwprintw( w(), 1, 2, "%s", _line1.c_str());
         ::mvwprintw( w(), 2, 2, "%s", _line2.c_str());
         ::wrefresh( w());
      }

      void setLine1( const char * fmt... ) {
         va_list args1, args2;
         va_start( args1, fmt );
         va_copy( args2, args1 );
         std::vector<char> buf( 1 + std::vsnprintf( NULL, 0, fmt, args1 ));
         va_end( args1 );
         std::vsnprintf( buf.data(), buf.size(), fmt, args2 );
         va_end( args2 );
         _line1 = buf.data();
         render();
      }

      void setLine2( const char * fmt... ) {
         va_list args1, args2;
         va_start( args1, fmt );
         va_copy( args2, args1 );
         std::vector<char> buf( 1 + std::vsnprintf( NULL, 0, fmt, args1 ));
         va_end( args1 );
         std::vsnprintf( buf.data(), buf.size(), fmt, args2 );
         va_end( args2 );
         _line2 = buf.data();
         render();
      }

   private:

      std::string _line1;
      std::string _line2;
   };
}
