#include "ESP8266WiFi.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

WiFiClient::WiFiClient() :
   _socket( 0 ),
   _timeout( 0 )
{}

WiFiClient::WiFiClient( int socket ) :
   _socket( socket ),
   _timeout( 0 )
{}

size_t WiFiClient::write( const uint8_t * value, size_t count ) {
   return ::write( _socket, value, count );
}

size_t WiFiClient::print( const char * value ) {
   return ::write( _socket, value, ::strlen( value ));
}

size_t WiFiClient::println( unsigned int value, int base /* = DEC */) {
   char tmp[40];
   if( base == DEC ) {
      ::snprintf( tmp, sizeof( tmp ), "%d\n", value );
   }
   else if( base == HEX ) {
      ::snprintf( tmp, sizeof( tmp ), "%x\n", value );
   }
   else if( base == OCT ) {
      ::snprintf( tmp, sizeof( tmp ), "%o\n", value );
   }
   return ::write( _socket, tmp, ::strlen( tmp ));
}

size_t WiFiClient::println( const char * value ) {
   ssize_t length = ::write( _socket, value, ::strlen( value ));
   return length + ::write( _socket, "\n", 1 );
}

size_t WiFiClient::println( void ) {
   return ::write( _socket, "\n", 1 );
}

WiFiClient::operator bool() {
   return _socket > 0;
}

void WiFiClient::setTimeout( unsigned long timeout ) {
   _timeout = timeout;
}

int WiFiClient::available() {
   return 1;
}

uint8_t WiFiClient::connected() {
   return 1;
}

size_t WiFiClient::readBytes( char * buffer, size_t length ) {
   ssize_t count = 0;
   do {
      count = ::read( _socket, (void *)buffer, (unsigned long)length );
      if( count < 0 ) {
         if(( errno != EWOULDBLOCK )&&( errno != EAGAIN )) {
            return count;
         }
         ::delay( _timeout );
      }
   } while( count < 0 );
   return count;
}

void WiFiClient::flush() {
}

void WiFiClient::stop() {
}
