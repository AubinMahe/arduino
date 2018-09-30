#include "Arduino.h"

String::String( const char * s ) :
   _string( s )
{}

const char * String::c_str() const {
   return _string.c_str();
}

String String::operator + ( const String & s ) const {
   return ( _string + s._string ).c_str();
}
