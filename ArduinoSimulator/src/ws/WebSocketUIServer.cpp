#include "WebSocketUIServer.h"
#include "WebSocketServer_Impl.h"

#undef min
#undef max
#include <iomanip>

using namespace ws;

WebSocketUIServer::WebSocketUIServer( int port ) :
   _ui( new WebSocketServer_Impl( port ))
{}

WebSocketUIServer:: ~ WebSocketUIServer() {
}

sim::SimuStatus WebSocketUIServer::getStatus( void ) const {
   return _ui->getStatus();
}

//-- Digital I/O -------------------------------------------------------------

int WebSocketUIServer::digitalRead( uint8_t pin ) const {
   return _ui->digitalRead( pin );
}

void WebSocketUIServer::digitalWrite( uint8_t pin, uint8_t hiOrLo ) const {
   _ui->enqueue( [ this, pin, hiOrLo ] () { _ui->digitalWrite( pin, hiOrLo ); });
}

void WebSocketUIServer::pinMode( uint8_t pin, uint8_t mode ) const {
   _ui->enqueue( [ this, pin, mode ] () { _ui->pinMode( pin, mode ); });
}

//-- Analog I/O --------------------------------------------------------------

int WebSocketUIServer::analogRead( uint8_t pin ) const {
   return _ui->analogRead( pin );
}

void WebSocketUIServer::analogReference( uint8_t mode ) const {
   _ui->enqueue( [ this, mode ] () { _ui->analogReference( mode ); });
}

void WebSocketUIServer::analogWrite( uint8_t pin, int value ) const {
   _ui->enqueue( [ this, pin, value ] () { _ui->analogWrite( pin, value ); });
}

//-- Advanced I/O ------------------------------------------------------------

void WebSocketUIServer::noTone( uint8_t ) const {/* NOT IMPLEMENTED */}
void WebSocketUIServer::tone( uint8_t, unsigned int, unsigned long ) const {/* NOT IMPLEMENTED */}

//-- External Interrupts -----------------------------------------------------

void WebSocketUIServer::attachInterrupt( uint8_t pin, void (* ISR )(void), uint8_t mode ) const {
   _ui->attachInterrupt( pin, ISR, mode );
}

void WebSocketUIServer::detachInterrupt( uint8_t pin ) const {
   _ui->detachInterrupt( pin );
}

//-- Communication -----------------------------------------------------------

size_t WebSocketUIServer::print( const char value[] ) const {
   if( _serialTeeStderr ) {
      ::fprintf( stderr, "%s", value );
   }
   _ui->enqueue( [ this, value ] () { _ui->print( value ? value : "" ); });
   return value ? ::strlen( value ) : 6;
}

size_t WebSocketUIServer::print( char value ) const {
   if( _serialTeeStderr ) {
      ::fprintf( stderr, "%c", value );
   }
   _ui->enqueue( [ this, value ] () { char tmp[2] = { value, '\0' }; _ui->print( tmp ); });
   return 1;
}

size_t WebSocketUIServer::print( uint8_t value, int base ) const {
   return print((long)value, base );
}

size_t WebSocketUIServer::print( int value, int base ) const {
   return print((long)value, base );
}

size_t WebSocketUIServer::print( unsigned int value, int base ) const{
   return print((unsigned long)value, base );
}

size_t WebSocketUIServer::print( long value, int base ) const{
   if( _serialTeeStderr ) {
      std::stringstream ss;
      if( base == OCT ) {
         ss << std::oct << value;
      }
      else if( base == HEX ) {
         ss << std::hex << value;
      }
      else {
         ss << value;
      }
      ::fprintf( stderr, "%s", ss.str().c_str());
   }
   _ui->enqueue( [ this, value, base ] () {
      std::stringstream ss;
      if( base == OCT ) {
         ss << std::oct << value;
      }
      else if( base == HEX ) {
         ss << std::hex << value;
      }
      else {
         ss << value;
      }
      _ui->print( ss.str());
   });
   return ::log( abs( value ));
}

size_t WebSocketUIServer::print( unsigned long value, int base ) const{
   if( _serialTeeStderr ) {
      std::stringstream ss;
      if( base == OCT ) {
         ss << std::oct << value;
      }
      else if( base == HEX ) {
         ss << std::hex << value;
      }
      else {
         ss << value;
      }
      ::fprintf( stderr, "%s", ss.str().c_str());
   }
   _ui->enqueue( [ this, value, base ] () {
      std::stringstream ss;
      if( base == OCT ) {
         ss << std::oct << value;
      }
      else if( base == HEX ) {
         ss << std::hex << value;
      }
      else {
         ss << value;
      }
      _ui->print( ss.str());
   });
   return ::log( abs( value ));
}

size_t WebSocketUIServer::print( double value, int prec ) const{
   if( _serialTeeStderr ) {
      std::stringstream ss;
      ss << std::setprecision( prec ) << value;
      ::fprintf( stderr, "%s", ss.str().c_str());
   }
   _ui->enqueue( [ this, value, prec ] () {
      std::stringstream ss;
      ss << std::setprecision( prec ) << value;
      _ui->print( ss.str());
   });
   return ::log( abs( value ));
}

size_t WebSocketUIServer::println( const char value[] ) const {
   if( _serialTeeStderr ) {
      ::fprintf( stderr, "%s\n", value );
   }
   _ui->enqueue( [ this, value ] () { _ui->println( value ? value : "" ); });
   return value ? ::strlen( value ) : 7;
}

size_t WebSocketUIServer::println( char value ) const {
   if( _serialTeeStderr ) {
      ::fprintf( stderr, "%c\n", value );
   }
   _ui->enqueue( [ this, value ] () { char tmp[2] = { value, '\0' }; _ui->println( tmp ); });
   return 1;
}

size_t WebSocketUIServer::println( uint8_t value, int base ) const {
   return println((long)value, base );
}

size_t WebSocketUIServer::println( int value, int base ) const {
   return println((long)value, base );
}

size_t WebSocketUIServer::println( unsigned int value, int base ) const{
   return println((unsigned long)value, base );
}

size_t WebSocketUIServer::println( long value, int base ) const{
   if( _serialTeeStderr ) {
      std::stringstream ss;
      if( base == OCT ) {
         ss << std::oct << value;
      }
      else if( base == HEX ) {
         ss << std::hex << value;
      }
      else {
         ss << value;
      }
      ::fprintf( stderr, "%s\n", ss.str().c_str());
   }
   _ui->enqueue( [ this, value, base ] () {
      std::stringstream ss;
      if( base == OCT ) {
         ss << std::oct << value;
      }
      else if( base == HEX ) {
         ss << std::hex << value;
      }
      else {
         ss << value;
      }
      _ui->println( ss.str());
   });
   return ::log( abs( value ));
}

size_t WebSocketUIServer::println( unsigned long value, int base ) const{
   if( _serialTeeStderr ) {
      std::stringstream ss;
      if( base == OCT ) {
         ss << std::oct << value;
      }
      else if( base == HEX ) {
         ss << std::hex << value;
      }
      else {
         ss << value;
      }
      ::fprintf( stderr, "%s\n", ss.str().c_str());
   }
   _ui->enqueue( [ this, value, base ] () {
      std::stringstream ss;
      if( base == OCT ) {
         ss << std::oct << value;
      }
      else if( base == HEX ) {
         ss << std::hex << value;
      }
      else {
         ss << value;
      }
      _ui->println( ss.str());
   });
   return ::log( abs( value ));
}

size_t WebSocketUIServer::println( double value, int prec ) const{
   if( _serialTeeStderr ) {
      std::stringstream ss;
      ss << std::setprecision( prec ) << value;
      ::fprintf( stderr, "%s\n", ss.str().c_str());
   }
   _ui->enqueue( [ this, value, prec ] () {
      std::stringstream ss;
      ss << std::setprecision( prec ) << value;
      _ui->println( ss.str());
   });
   return ::log( abs( value ));
}

size_t WebSocketUIServer::println( void ) const{
   return println("");
}

//-- Servo -------------------------------------------------------------------

void WebSocketUIServer::servoAttach( uint8_t pin ) const {
   _ui->enqueue( [this,pin] () { _ui->servoAttach( pin ); });
}

void WebSocketUIServer::servoWrite( uint8_t pin, int value ) const {
   _ui->enqueue( [this,pin,value]() { _ui->servoWrite( pin, value ); });
}

void WebSocketUIServer::servoDetach( uint8_t pin ) const {
   _ui->enqueue( [this,pin]() { _ui->servoDetach( pin ); });
}
