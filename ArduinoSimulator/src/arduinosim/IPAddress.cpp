#include "ESP8266WiFi.h"

#include <string.h>

IPAddress::IPAddress() {
   _address.bytes[0] = 192;
   _address.bytes[1] = 168;
   _address.bytes[2] =   1;
   _address.bytes[3] =   3;
}

size_t IPAddress::printTo( Print & p ) const {
   size_t length = 0;
   length += p.print( _address.bytes[0] );
   length += p.print( '.' );
   length += p.print( _address.bytes[1] );
   length += p.print( '.' );
   length += p.print( _address.bytes[2] );
   length += p.print( '.' );
   length += p.print( _address.bytes[3] );
   return length;
}
