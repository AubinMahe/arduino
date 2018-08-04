#pragma once

#include <stdint.h>
#include <ctype.h>
#include <stdlib.h>

//-- Digital I/O -------------------------------------------------------------

#define HIGH   0x1
#define LOW    0x0
#define INPUT  0x0
#define OUTPUT 0x1

int  digitalRead ( uint8_t pin );
void digitalWrite( uint8_t pin, uint8_t hiOrLo );
void pinMode     ( uint8_t pin, uint8_t inOrOut );

//-- Analog I/O --------------------------------------------------------------

int  analogRead     ( uint8_t pin );
void analogReference( uint8_t mode);
void analogWrite    ( uint8_t pin, int value );

//-- Zero, Due & MKR Family --------------------------------------------------

//analogReadResolution()
//analogWriteResolution()

//-- Advanced I/O ------------------------------------------------------------

void          noTone     ( uint8_t pin );
unsigned long pulseIn    ( uint8_t pin, uint8_t state, unsigned long timeout );
unsigned long pulseInLong( uint8_t pin, uint8_t state, unsigned long timeout );
uint8_t       shiftIn    ( uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder );
void          shiftOut   ( uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val );
void          tone       ( uint8_t pin, unsigned int frequency, unsigned long duration = 0 );

//-- Time --------------------------------------------------------------------

void          delay            ( unsigned long );
void          delayMicroseconds( unsigned int us );
unsigned long micros           ( void );
unsigned long millis           ( void );

//-- Math --------------------------------------------------------------------

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define EULER 2.718281828459045235360287471352

#ifdef abs
#undef abs
#endif
#define abs(x) ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return out_min + (x - in_min) * (out_max - out_min) / (in_max - in_min);
}
#define max(a,b) ((a)>(b)?(a):(b))
#define min(a,b) ((a)<(b)?(a):(b))
#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
//pow()
#define sq(x) ((x)*(x))
//sqrt()

//-- Trigonometry ------------------------------------------------------------

#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

//cos()
//sin()
//tan()
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)

//-- Characters --------------------------------------------------------------

inline bool isAlpha           ( char c ) { return ::isalpha ( c ); }
inline bool isAlphaNumeric    ( char c ) { return ::isalnum ( c ); }
inline bool isAscii           ( char c ) { return   isascii ( c ); }
inline bool isControl         ( char c ) { return ::iscntrl ( c ); }
inline bool isDigit           ( char c ) { return ::isdigit ( c ); }
inline bool isGraph           ( char c ) { return ::isgraph ( c ); }
inline bool isHexadecimalDigit( char c ) { return ::isxdigit( c ); }
inline bool isLowerCase       ( char c ) { return ::islower ( c ); }
inline bool isPrintable       ( char c ) { return ::isprint ( c ); }
inline bool isPunct           ( char c ) { return ::ispunct ( c ); }
inline bool isSpace           ( char c ) { return ::isspace ( c ); }
inline bool isUpperCase       ( char c ) { return ::isupper ( c ); }
inline bool isWhitespace      ( char c ) { return ::isblank ( c ); }

//-- Random Numbers ----------------------------------------------------------

inline long random( long max ) {
   return map( ::random(), 0, RAND_MAX, 0, max );
}

inline long random( long min, long max ) {
   return map( ::random(), 0, RAND_MAX, min, max );
}

inline void randomSeed( long seed ) {
   ::srandom( seed );
}

//-- Bits and Bytes ----------------------------------------------------------

#define bit(b) (1UL << (b))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define highByte(w) ((uint8_t) ((w) >> 8))
#define lowByte(w) ((uint8_t) ((w) & 0xff))

//-- External Interrupts -----------------------------------------------------

void attachInterrupt( uint8_t interrupt, void (* ISR )(void), int mode );
void detachInterrupt( uint8_t interrupt );

//-- Interrupts --------------------------------------------------------------

//interrupts
//noInterrupts

//-- Communication -----------------------------------------------------------

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

//-- USB ---------------------------------------------------------------------

//class Keyboard
//class Mouse

//-- Sketch ------------------------------------------------------------------

void loop( void );
void setup( void );