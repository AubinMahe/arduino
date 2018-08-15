#pragma once

#include "../IUI.h"

namespace ws {

   class WebSocketServer_Impl;
   class WebSocketUIServer : public IUI {
   public:

      WebSocketUIServer( int port );

      virtual ~ WebSocketUIServer();

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

      WebSocketServer_Impl * _ui;
   };
}
