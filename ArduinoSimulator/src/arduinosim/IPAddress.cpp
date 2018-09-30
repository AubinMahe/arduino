#include "ESP8266WiFi.h"
#include "IPAddressValue.h"

#include <string.h>

IPAddress::IPAddress() {
   _address.bytes[0] = LOCAL_IP_ADDRESS[0];
   _address.bytes[1] = LOCAL_IP_ADDRESS[1];
   _address.bytes[2] = LOCAL_IP_ADDRESS[2];
   _address.bytes[3] = LOCAL_IP_ADDRESS[3];
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

std::string IPAddress::toString( void ) const {
   char buffer[80];
   snprintf( buffer, sizeof( buffer ), "%d.%d.%d.%d",
      _address.bytes[0], _address.bytes[1], _address.bytes[2], _address.bytes[3] );
   return buffer;
}
