#pragma once

#include "../IUI.h"

namespace ncurses {

   class UI_Impl;
   class UI : public sim::IUI {
   public:

      UI();

      virtual ~ UI();

   public:

      virtual sim::SimuStatus getStatus( void ) const;

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

      virtual size_t print( const char    value[]         ) const;
      virtual size_t print( char          value           ) const;
      virtual size_t print( uint8_t       value, int base ) const;
      virtual size_t print( int           value, int base ) const;
      virtual size_t print( unsigned int  value, int base ) const;
      virtual size_t print( long          value, int base ) const;
      virtual size_t print( unsigned long value, int base ) const;
      virtual size_t print( double        value, int prec ) const;

      virtual size_t println( const char    value[]         ) const;
      virtual size_t println( char          value           ) const;
      virtual size_t println( uint8_t       value, int base ) const;
      virtual size_t println( int           value, int base ) const;
      virtual size_t println( unsigned int  value, int base ) const;
      virtual size_t println( long          value, int base ) const;
      virtual size_t println( unsigned long value, int base ) const;
      virtual size_t println( double        value, int prec ) const;
      virtual size_t println( void                          ) const;

      //-- Servo -------------------------------------------------------------------

      virtual void servoAttach( uint8_t pin ) const;

      virtual void servoWrite( uint8_t pin, int value ) const;

      virtual void servoDetach( uint8_t pin ) const;

   private:

      UI_Impl * _ui;
   };
}
