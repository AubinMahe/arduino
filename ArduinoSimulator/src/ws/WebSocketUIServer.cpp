#include "WebSocketUIServer.h"
#include "WebSocketServer_Impl.h"

using namespace ws;

WebSocketUIServer::WebSocketUIServer( int port ) :
   _ui( new WebSocketServer_Impl( port ))
{}

WebSocketUIServer:: ~ WebSocketUIServer() {
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

void WebSocketUIServer::print( const char * line ) const {
   _ui->enqueue( [ this, line ] () { _ui->print( line ); });
}

void WebSocketUIServer::println( const char * line ) const {
   _ui->enqueue( [ this, line ] () { _ui->println( line ); });
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
