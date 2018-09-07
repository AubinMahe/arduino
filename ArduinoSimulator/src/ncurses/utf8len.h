#pragma once

#undef min
#undef max
#include <string>

namespace hpms {

   unsigned utf8len( const char * utf8 );
   unsigned utf8len( const std::string & utf8 );
}
