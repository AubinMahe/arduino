#include "utf8len.h"

unsigned hpms::utf8len( const char * utf8 ) {
   unsigned len = 0U;
   while( *utf8 ) {
      len += ( *utf8++ & 0xc0 ) != 0x80;
   }
   return len;
}

unsigned hpms::utf8len( const std::string & utf8 ) {
   return hpms::utf8len( utf8.c_str());
}
