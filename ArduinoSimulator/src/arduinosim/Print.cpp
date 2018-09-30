#include "Arduino.h"
#include "../IUI.h"

size_t Print::print( const char value[] ) {
   return sim::IUI::_theUI->print( value );
}
size_t Print::print( char value ) {
   return sim::IUI::_theUI->print( value );
}
size_t Print::print( unsigned char value, int base /* = DEC */) {
   return sim::IUI::_theUI->print( value, base );
}
size_t Print::print( int value, int base /* = DEC */) {
   return sim::IUI::_theUI->print( value, base );
}
size_t Print::print( unsigned int value, int base /* = DEC */) {
   return sim::IUI::_theUI->print( value, base );
}
size_t Print::print( long value, int base /* = DEC */) {
   return sim::IUI::_theUI->print( value, base );
}
size_t Print::print( unsigned long value, int base /* = DEC */) {
   return sim::IUI::_theUI->print( value, base );
}
size_t Print::print( double value, int prec /* = 2 */) {
   return sim::IUI::_theUI->print( value, prec );
}
size_t Print::print( const String & value ) {
   return sim::IUI::_theUI->print( value.c_str());
}
size_t Print::print( const Printable & value ) {
   return value.printTo( *this );
}

size_t Print::println( const char value[] ) {
   return sim::IUI::_theUI->println( value );
}
size_t Print::println( unsigned char value, int base /* = DEC */ ) {
   return sim::IUI::_theUI->println( value, base );
}
size_t Print::println( int value, int base /* = DEC */) {
   return sim::IUI::_theUI->println( value, base );
}
size_t Print::println( unsigned long value, int base /* = DEC */ ) {
   return sim::IUI::_theUI->println( value, base );
}
size_t Print::println( void ) {
   return sim::IUI::_theUI->println();
}
size_t Print::println( const String & value ) {
   return sim::IUI::_theUI->println( value.c_str());
}
size_t Print::println( const Printable & value ) {
   value.printTo( *this );
   return sim::IUI::_theUI->println();
}
