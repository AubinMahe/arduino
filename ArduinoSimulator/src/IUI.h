#pragma once

#include <stdint.h>

class IUI {
public:

   IUI( void ) {}

   virtual ~ IUI() {}

public:

   //-- Digital I/O ----------------------------------------------------------

   virtual int digitalRead( uint8_t pin ) const = 0;

   virtual void digitalWrite( uint8_t pin, uint8_t hiOrLo ) const = 0;

   virtual void pinMode( uint8_t pin, uint8_t inOrOut ) const = 0;

   //-- Analog I/O -----------------------------------------------------------

   virtual int analogRead( uint8_t pin ) const = 0;

   virtual void analogReference( uint8_t mode ) const = 0;

   virtual void analogWrite( uint8_t pin, int value ) const = 0;

   //-- Advanced I/O ---------------------------------------------------------

   virtual void noTone( uint8_t pin ) const = 0;

   virtual void tone( uint8_t pin, unsigned int frequency, unsigned long duration ) const = 0;

   //-- External Interrupts --------------------------------------------------

   virtual void attachInterrupt( uint8_t pin, void (* ISR )(void), uint8_t mode ) const = 0;

   virtual void detachInterrupt( uint8_t pin ) const = 0;

   //-- Communication --------------------------------------------------------

   virtual void print( const char * s ) const = 0;

   virtual void println( const char * s ) const = 0;

   //-- Servo -------------------------------------------------------------------

   virtual void servoAttach( uint8_t pin ) const = 0;

   virtual void servoWrite( uint8_t pin, int value ) const = 0;

   virtual void servoDetach( uint8_t pin ) const = 0;

private:
   IUI( const IUI & );
   IUI & operator = ( const IUI & );
};
