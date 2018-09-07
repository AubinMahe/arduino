#pragma once

#undef min
#undef max
#include <string>
#include <vector>

namespace hpms {

   bool getFileContents( const char * name, std::vector<std::string> & lines );
}
