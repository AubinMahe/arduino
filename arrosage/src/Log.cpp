#include "Arduino.h"
#include "Log.h"

#include <stdarg.h>
#include <stdio.h>

void hpms::Log( const char * format ... ) {
   va_list args;
   va_start( args, format );
   char buffer[1000];
   ::vsnprintf( buffer, sizeof( buffer ), format, args );
   va_end( args );
   Serial.print( millis());
   Serial.print( ": " );
   Serial.println( buffer );
}
