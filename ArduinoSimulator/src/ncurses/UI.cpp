#include "UI_Impl.h"

using namespace ncurses;

UI::UI() :
   _ui( new UI_Impl())
{}

UI:: ~ UI() {
   delete _ui;
}

sim::SimuStatus UI::getStatus( void ) const {
   return _ui->getStatus();
}

//-- Digital I/O ----------------------------------------------------------

int UI::digitalRead( uint8_t pin ) const {
   return _ui->ctrl().digitalRead( pin );
}

void UI::digitalWrite( uint8_t pin, uint8_t hiOrLo ) const {
   _ui->enqueue( [ this, pin, hiOrLo ] () { _ui->ctrl().digitalWrite( pin, hiOrLo ); });
}

void UI::pinMode( uint8_t pin, uint8_t mode ) const {
   _ui->enqueue( [ this, pin, mode ] () { _ui->ctrl().pinMode( pin, mode, _ui ); });
}

//-- Analog I/O -----------------------------------------------------------

int UI::analogRead( uint8_t pin ) const {
   return _ui->ctrl().analogRead( pin );
}

void UI::analogReference( uint8_t mode ) const {
   _ui->enqueue( [ this, mode ] () { _ui->ctrl().analogReference( mode ); });
}

void UI::analogWrite( uint8_t pin, int value ) const {
   _ui->enqueue( [ this, pin, value ] () { _ui->ctrl().analogWrite( pin, value ); });
}

//-- Advanced I/O ---------------------------------------------------------

void UI::noTone( uint8_t pin ) const {
   _ui->enqueue( [ this, pin ] () { _ui->ctrl().noTone( pin ); });
}

void UI::tone( uint8_t pin, unsigned int frequency, unsigned long duration ) const {
   _ui->enqueue( [ this, pin, frequency, duration ] () { _ui->ctrl().tone( pin, frequency, duration ); });
}

//-- External Interrupts --------------------------------------------------

void UI::attachInterrupt( uint8_t pin, void (* ISR )(void), uint8_t mode ) const {
   _ui->attachInterrupt( pin, ISR, mode );
}

void UI::detachInterrupt( uint8_t pin ) const {
   _ui->detachInterrupt( pin );
}

//-- Communication --------------------------------------------------------

size_t UI::print( const char value[] ) const {
   _ui->enqueue( [ this, value ] () { _ui->log().print( value ); });
   return strlen( value );
}

size_t UI::print( unsigned char value, int base ) const {
   return print((long)value, base );
}

size_t UI::print( int value, int base ) const {
   return print((long)value, base );
}

size_t UI::print( unsigned int value, int base ) const{
   return print((unsigned long)value, base );
}

size_t UI::print( long value, int base ) const{
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
      _ui->log().print( ss.str());
   });
   return ::log( abs( value ));
}

size_t UI::print( unsigned long value, int base ) const{
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
      _ui->log().print( ss.str());
   });
   return ::log( abs( value ));
}

size_t UI::print( double value, int prec ) const{
   _ui->enqueue( [ this, value, prec ] () {
      std::stringstream ss;
      ss << std::setprecision( prec ) << value;
      _ui->log().print( ss.str());
   });
   return ::log( abs( value ));
}

size_t UI::println( const char value[] ) const {
   _ui->enqueue( [ this, value ] () { _ui->log().print( std::string( value ) + "\n" ); });
   return strlen( value );
}

size_t UI::println( unsigned char value, int base ) const {
   return println((long)value, base );
}

size_t UI::println( int value, int base ) const {
   return println((long)value, base );
}

size_t UI::println( unsigned int value, int base ) const{
   return println((unsigned long)value, base );
}

size_t UI::println( long value, int base ) const{
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
      _ui->log().print( ss.str() + "\n" );
   });
   return ::log( abs( value ));
}

size_t UI::println( unsigned long value, int base ) const{
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
      _ui->log().print( ss.str() + "\n" );
   });
   return ::log( abs( value ));
}

size_t UI::println( double value, int prec ) const{
   _ui->enqueue( [ this, value, prec ] () {
      std::stringstream ss;
      ss << std::setprecision( prec ) << value;
      _ui->log().print( ss.str() + "\n" );
   });
   return ::log( abs( value ));
}

size_t UI::println( void ) const{
   _ui->enqueue( [ this ] () {
      _ui->log().print( "\n" );
   });
   return 1;
}

//-- Servo -------------------------------------------------------------------

void UI::servoAttach( uint8_t pin ) const {
   _ui->enqueue( [this,pin] () { _ui->ctrl().servoAttach( pin ); });
}

void UI::servoWrite( uint8_t pin, int value ) const {
   _ui->enqueue( [this,pin,value]() { _ui->ctrl().servoWrite( pin, value ); });
}

void UI::servoDetach( uint8_t pin ) const {
   _ui->enqueue( [this,pin]() { _ui->ctrl().servoDetach( pin ); });
}
