#include <Arduino.h>
#include <Servo.h>

#include "java/UIProxy.h"
#include "ncurses/UI.h"
#include "ws/WebSocketUIServer.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

#undef min
#undef max
#include <iostream>

#define UI_PROXY_PORT 2416

sim::IUI * proxy;

//-- Digital I/O -------------------------------------------------------------

int digitalRead ( uint8_t pin ) {
   return proxy->digitalRead( pin );
}

void digitalWrite( uint8_t pin, uint8_t hiOrLo ) {
   proxy->digitalWrite( pin, hiOrLo );
}

void pinMode( uint8_t pin, uint8_t inOrOut ) {
   proxy->pinMode( pin, inOrOut );
}

//-- Analog I/O --------------------------------------------------------------

int analogRead( uint8_t pin ) {
   return proxy->analogRead( pin );
}

void analogReference( uint8_t mode) {
   proxy->analogReference( mode );
}

void analogWrite( uint8_t pin, int value ) {
   proxy->analogWrite( pin, value );
}

//-- Advanced I/O ------------------------------------------------------------

void noTone( uint8_t pin ) {
   proxy->noTone( pin );
}

unsigned long pulseIn( uint8_t/*pin*/, uint8_t/*state*/, unsigned long/*timeout*/) {
   // TODO
   return 0U;
}

unsigned long pulseInLong( uint8_t/*pin*/, uint8_t/*state*/, unsigned long/*timeout*/) {
   // TODO
   return 0U;
}

uint8_t shiftIn( uint8_t/*dataPin*/, uint8_t/*clockPin*/, uint8_t/*bitOrder*/) {
   // TODO
   return 0U;
}

void shiftOut( uint8_t/*dataPin*/, uint8_t/*clockPin*/, uint8_t/*bitOrder*/, uint8_t/*val*/) {
   // TODO
}

void tone( uint8_t pin, unsigned int frequency, unsigned long duration ) {
   proxy->tone( pin, frequency, duration );
}

//-- Time --------------------------------------------------------------------

void delay( unsigned long ms ) {
   timespec spec;
   spec.tv_sec  = ms / 1000;
   spec.tv_nsec = 1000 * 1000 * ( ms - spec.tv_sec * 1000 );
   nanosleep( &spec, 0 );
}

void delayMicroseconds( unsigned int us ) {
   timespec spec;
   spec.tv_sec  = us / (1000 * 1000);
   spec.tv_nsec = 1000 * ( us - spec.tv_sec * 1000 * 1000 );
   nanosleep( &spec, 0 );
}

unsigned long micros( void ) {
   timeval tv;
   gettimeofday( &tv, 0 );
   return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}

unsigned long millis( void ) {
   timeval tv;
   gettimeofday( &tv, 0 );
   return tv.tv_sec * 1000 + tv.tv_usec / 1000 ;
}

//-- Math --------------------------------------------------------------------

//-- Trigonometry ------------------------------------------------------------

//-- Characters --------------------------------------------------------------

//-- Random Numbers ----------------------------------------------------------

//-- Bits and Bytes ----------------------------------------------------------

//-- External Interrupts -----------------------------------------------------

void attachInterrupt( uint8_t interrupt, void (* ISR )(void), int mode ) {
   if( interrupt == 0 ) {
      proxy->attachInterrupt( 2, ISR, mode );
   }
   else if( interrupt == 1 ) {
      proxy->attachInterrupt( 3, ISR, mode );
   }
}

void detachInterrupt( uint8_t interrupt ) {
   if( interrupt == 0 ) {
      proxy->detachInterrupt( 2 );
   }
   else if( interrupt == 1 ) {
      proxy->detachInterrupt( 3 );
   }
}

//-- Interrupts --------------------------------------------------------------

//-- Communication -----------------------------------------------------------

size_t Print::print( const char value[] ) {
   return proxy->print( value );
}

size_t Print::print( unsigned long value, int base /* = DEC */ ) {
   return proxy->print( value, base );
}

size_t Print::println( const char value[] ) {
   return proxy->println( value );
}

size_t Print::println( unsigned long value, int base /* = DEC */ ) {
   return proxy->println( value, base );
}

size_t Print::println( void ) {
   return proxy->println();
}

void HardwareSerial::begin( unsigned long, uint8_t ) {
}

HardwareSerial Serial;

//-- Servo -------------------------------------------------------------------

Servo::Servo() {
   _pin = (uint8_t)-1U;
}

uint8_t Servo::attach( int pin ) {
   proxy->servoAttach( pin );
   _pin = pin;
   return 1;
}

uint8_t Servo::attach( int pin, int/*min*/, int/*max*/) {
   return attach( pin );
}

void Servo::detach() {
   proxy->servoDetach( _pin );
   _pin = -1;
}

void Servo::write( int value ) {
   proxy->servoWrite( _pin, value );
}

void Servo::writeMicroseconds( int ) {
}

int Servo::read() {
   return 0;
}

int Servo::readMicroseconds() {
   return 0;
}

bool Servo::attached() {
   return _pin != (uint8_t)-1U;
}

//-- Main --------------------------------------------------------------------

//extern void jsontest( void );

int main( int argc, char * argv[] ) {
//   jsontest();
   proxy = 0;
   if( argc > 1 && 0 == strcmp( "--ui=ncurses", argv[1] )) {
      proxy = new ncurses::UI();
   }
   else if( argc > 1 && 0 == strcmp( "--ui=java", argv[1] )) {
      proxy = new JavaUIProxy( UI_PROXY_PORT );
   }
   else if( argc > 1 && 0 == strcmp( "--ui=ws", argv[1] )) {
      proxy = new ws::WebSocketUIServer( UI_PROXY_PORT );
   }
   if( proxy ) {
      timespec spec;
      int status = sim::E_RUNNING;
      while(( status = proxy->getStatus()) != sim::E_ENDED ) {
         setup();
         while(( status = proxy->getStatus()) == sim::E_RUNNING ) {
            loop();
            spec.tv_sec  = 0;
            spec.tv_nsec = 20 * 1000 * 1000; // 20 ms
            nanosleep( &spec, 0 );
         }
      }
   }
   else {
      fprintf( stderr, "usage: %s --ui={ncurses|java|ws}\n", argv[0] );
      return EXIT_FAILURE;
   }
   return EXIT_SUCCESS;
}
