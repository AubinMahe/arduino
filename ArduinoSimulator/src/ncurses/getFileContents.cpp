#include "getFileContents.h"

#include <string.h>

std::vector<std::string> hpms::getFileContents( const char * name ) {
   std::vector<std::string> lines;
   FILE * instructions = fopen( name, "r" );
   if( instructions ) {
      char line[1024];
      while( fgets( line, sizeof( line ), instructions )) {
         lines.push_back( strtok( line, "\n" ));
      }
      fclose( instructions );
   }
   return lines;
}
