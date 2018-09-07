#include "getFileContents.h"

#include <string.h>

bool hpms::getFileContents( const char * name, std::vector<std::string> & lines ) {
   FILE * instructions = fopen( name, "r" );
   if( ! instructions ) {
      return false;
   }
   char line[1024];
   while( fgets( line, sizeof( line ), instructions )) {
      lines.push_back( strtok( line, "\n" ));
   }
   fclose( instructions );
   return true;
}
