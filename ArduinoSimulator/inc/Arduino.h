#pragma once

#include <stdint.h>

class Print {
public:

   virtual ~ Print() {}

   void print( const char * );

   void println( const char * );

   virtual void flush() { /* Empty implementation for backward compatibility */ }
};

class Stream : public Print {
public:

};

#define SERIAL_8N1 0x06

class HardwareSerial : public Stream {
public:

   void begin( unsigned long baud, uint8_t );
   void begin( unsigned long baud ) { begin( baud, SERIAL_8N1 ); }
};

extern HardwareSerial Serial;

void tone  ( uint8_t pin, unsigned int frequency, unsigned long duration = 0 );
void noTone( uint8_t pin );

unsigned long millis( void );

#define INPUT  0x0
#define OUTPUT 0x1

void pinMode     ( uint8_t pin, uint8_t inOrOut );

#define HIGH 0x1
#define LOW  0x0

void digitalWrite( uint8_t pin, uint8_t hiOrLo );
int  digitalRead ( uint8_t pin );
