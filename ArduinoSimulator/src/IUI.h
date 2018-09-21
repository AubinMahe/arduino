#pragma once

#include <stdint.h>
#include <stddef.h>

#undef min
#undef max
#include <string>
#include <vector>

namespace sim {

   enum SimuStatus {

      E_RESET,
      E_RUNNING,
      E_ENDED,
   };

   class IUI {
   public:

      IUI( void ) :
         _serialTeeStderr( false )
      {
         _theUI = this;
      }

      virtual ~ IUI() {}

      void serialTeeStderr( bool tee ) {
         _serialTeeStderr = tee;
      }

      void addArgument( const char * argument ) {
         _arguments.push_back( argument );
      }

      const std::vector<std::string> & getArguments( void ) {
         return _arguments;
      }

   public:

      virtual SimuStatus getStatus( void ) const = 0;

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

      virtual size_t print( const char    value[]         ) const = 0;
      virtual size_t print( char          value           ) const = 0;
      virtual size_t print( uint8_t       value, int base ) const = 0;
      virtual size_t print( int           value, int base ) const = 0;
      virtual size_t print( unsigned int  value, int base ) const = 0;
      virtual size_t print( long          value, int base ) const = 0;
      virtual size_t print( unsigned long value, int base ) const = 0;
      virtual size_t print( double        value, int prec ) const = 0;

      virtual size_t println( const char    value[]         ) const = 0;
      virtual size_t println( char          value           ) const = 0;
      virtual size_t println( uint8_t       value, int base ) const = 0;
      virtual size_t println( int           value, int base ) const = 0;
      virtual size_t println( unsigned int  value, int base ) const = 0;
      virtual size_t println( long          value, int base ) const = 0;
      virtual size_t println( unsigned long value, int base ) const = 0;
      virtual size_t println( double        value, int prec ) const = 0;
      virtual size_t println( void                          ) const = 0;

      //-- Servo -------------------------------------------------------------------

      virtual void servoAttach( uint8_t pin ) const = 0;

      virtual void servoWrite( uint8_t pin, int value ) const = 0;

      virtual void servoDetach( uint8_t pin ) const = 0;

   public:

      static IUI * _theUI;

   protected:

      bool                     _serialTeeStderr;
      std::vector<std::string> _arguments;

   private:
      IUI( const IUI & );
      IUI & operator = ( const IUI & );
   };
}
