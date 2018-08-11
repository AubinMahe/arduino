#pragma once

#include "../IUI.h"

#define UI_PROXY_PORT 2416

struct JavaUIProxyAttributes;
class JavaUIProxy : public IUI {
public:

   JavaUIProxy( unsigned short port );

public:

   //-- Digital I/O ----------------------------------------------------------

   virtual int digitalRead( uint8_t pin ) const;

   virtual void digitalWrite( uint8_t pin, uint8_t hiOrLo ) const;

   virtual void pinMode( uint8_t pin, uint8_t inOrOut ) const;

   //-- Analog I/O -----------------------------------------------------------

   virtual int analogRead( uint8_t pin ) const;

   virtual void analogReference( uint8_t mode ) const;

   virtual void analogWrite( uint8_t pin, int value ) const;

   //-- Advanced I/O ---------------------------------------------------------

   virtual void noTone( uint8_t pin ) const;

   virtual void tone( uint8_t pin, unsigned int frequency, unsigned long duration ) const;

   //-- External Interrupts --------------------------------------------------

   virtual void attachInterrupt( uint8_t pin, void (* ISR )(void), uint8_t mode ) const;

   virtual void detachInterrupt( uint8_t pin ) const;

   //-- Communication --------------------------------------------------------

   virtual void print( const char * s ) const;

   virtual void println( const char * s ) const;

   //-- Servo -------------------------------------------------------------------

   virtual void servoAttach( uint8_t pin ) const;

   virtual void servoWrite( uint8_t pin, int value ) const;

   virtual void servoDetach( uint8_t pin ) const;

private:

   void _recv() const;

   static inline void * recv( void * arg ) {
      ((const JavaUIProxy *)arg) -> _recv();
      return 0;
   }

   void send( int verb, const char * s ) const;

   void send( const char * buffer, unsigned short len ) const;

private:

   JavaUIProxyAttributes * a;
};
