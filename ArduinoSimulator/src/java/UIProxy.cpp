#include <Arduino.h>
#include <Servo.h>

#include "UIProxy.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#undef min
#undef max
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#define UI_PROXY_PORT 2416

struct JavaUIProxyAttributes {

   int                      socket;
   sockaddr_in              addr;
   uint8_t                  digital[16];
   int                      analog [16];
   uint8_t                  isr_0_mode;
   void( *                  isr_0_func )( void );
   uint8_t                  isr_1_mode;
   void( *                  isr_1_func )( void );
   sim::SimuStatus          status;
};

enum verb_t {

   E_NONE,
   E_RESET,

   //-- Digital I/O ----------------------------------------------------------

   E_DIGITAL_READ,
   E_DIGITAL_WRITE,
   E_PIN_MODE,

   //-- Analog I/O -----------------------------------------------------------

   E_ANALOG_READ,
   E_ANALOG_REFERENCE,
   E_ANALOG_WRITE,

   //-- Advanced I/O ---------------------------------------------------------

   E_NO_TONE,
   E_TONE,

   //-- External Interrupts --------------------------------------------------

   //-- Communication --------------------------------------------------------

   E_PRINT,
   E_PRINTLN,

   //-- Servo ----------------------------------------------------------------

   E_SERVO_ATTACH,
   E_SERVO_WRITE,
   E_SERVO_DETACH,

   //-- Exit -----------------------------------------------------------------

   E_EXIT
};

JavaUIProxy::JavaUIProxy( unsigned short port ) {
   a = new JavaUIProxyAttributes();
   ::memset( a, 0, sizeof( JavaUIProxyAttributes ));
   a->isr_0_mode = 0;
   a->isr_0_func = 0;
   a->isr_1_mode = 0;
   a->isr_1_func = 0;
   a->socket = ::socket( AF_INET, SOCK_DGRAM, 0 );
   if( a->socket < 0 ) {
      ::perror("socket()");
      ::exit( EXIT_FAILURE );
   }
   ::memset( &a->addr, 0, sizeof( a->addr ));
   a->addr.sin_family      = AF_INET;
   a->addr.sin_addr.s_addr = htonl( INADDR_ANY );
   a->addr.sin_port        = htons( port + 1 );
   int st = ::bind( a->socket, (sockaddr *)&a->addr, sizeof( a->addr ));
   if( st ) {
      ::perror("bind()");
      ::exit( EXIT_FAILURE );
   }
   a->addr.sin_port = htons( port );
   a->status = sim::E_RUNNING;
   pthread_t thread;
   if( ::pthread_create( &thread, 0, JavaUIProxy::recv, this )) {
      ::perror("pthread_create()");
      ::exit( EXIT_FAILURE );
   }
}

sim::SimuStatus JavaUIProxy::getStatus( void ) const {
   sim::SimuStatus status = a->status;
   if( status == sim::E_RESET ) {
      a->status = sim::E_RUNNING;
   }
   return status;
}

//-- Digital I/O ----------------------------------------------------------

int JavaUIProxy::digitalRead( uint8_t pin ) const {
   return a->digital[pin];
}

void JavaUIProxy::digitalWrite( uint8_t pin, uint8_t hiOrLo ) const {
   char   buffer[1+1+1];
   char * p = buffer;
   *p = E_DIGITAL_WRITE;
   p += 1;
   *((uint8_t*)p) = pin;
   p += 1;
   *((uint8_t*)p) = hiOrLo;
   send( buffer, sizeof( buffer ));
}

void JavaUIProxy::pinMode( uint8_t pin, uint8_t inOrOut ) const {
   char   buffer[1+1+1];
   char * p = buffer;
   *p = E_PIN_MODE;
   p += 1;
   *((uint8_t*)p) = pin;
   p += 1;
   *((uint8_t*)p) = inOrOut;
   send( buffer, sizeof( buffer ));
}

//-- Analog I/O -----------------------------------------------------------

int JavaUIProxy::analogRead( uint8_t pin ) const {
   return a->analog[pin];
}

void JavaUIProxy::analogReference( uint8_t mode ) const {
   char   buffer[1+1];
   char * p = buffer;
   *p = E_ANALOG_REFERENCE;
   p += 1;
   *((uint8_t*)p) = mode;
   send( buffer, sizeof( buffer ));
}

void JavaUIProxy::analogWrite( uint8_t pin, int value ) const {
   char   buffer[1+1+4];
   char * p = buffer;
   *p = E_ANALOG_WRITE;
   p += 1;
   *((uint8_t*)p) = pin;
   p += 1;
   *((int*)p) = ntohl( value );
   send( buffer, sizeof( buffer ));
}

//-- Advanced I/O ---------------------------------------------------------

void JavaUIProxy::noTone( uint8_t pin ) const {
   char   buffer[1+1];
   char * p = buffer;
   *p = E_NO_TONE;
   p += 1;
   *((uint8_t*)p) = pin;
   send( buffer, sizeof( buffer ));
}

void JavaUIProxy::tone( uint8_t pin, unsigned int frequency, unsigned long duration ) const {
   char   buffer[1+1+4+4];
   char * p = buffer;
   *p = E_TONE;
   p += 1;
   *((uint8_t*)p) = pin;
   p += 1;
   *((unsigned int *)p) = htonl( frequency );
   p += sizeof( unsigned int );
   *((unsigned int *)p) = htonl( duration );
   send( buffer, sizeof( buffer ));
}

//-- External Interrupts --------------------------------------------------

void JavaUIProxy::attachInterrupt( uint8_t pin, void (* ISR )(void), uint8_t mode ) const {
   if( pin == 2 ) {
      if( mode == RISING || mode == FALLING || mode == CHANGE ) {
         a->isr_0_mode = mode;
         a->isr_0_func = ISR;
      }
      else {
         a->isr_0_mode = 0;
         a->isr_0_func = 0;
      }
   }
   else if( pin == 3 ) {
      if( mode == RISING || mode == FALLING || mode == CHANGE ) {
         a->isr_1_mode = mode;
         a->isr_1_func = ISR;
      }
      else {
         a->isr_1_mode = 0;
         a->isr_1_func = 0;
      }
   }
}

void JavaUIProxy::detachInterrupt( uint8_t pin ) const {
   if( pin == 2 ) {
      a->isr_0_mode = 0;
      a->isr_0_func = 0;
   }
   else if( pin == 3 ) {
      a->isr_1_mode = 0;
      a->isr_1_func = 0;
   }
}

//-- Communication --------------------------------------------------------

size_t JavaUIProxy::print( const char value[] ) const {
   if( _serialTeeStderr ) {
      ::fprintf( stderr, "%s", value );
   }
   return send( E_PRINT, value );
}

size_t JavaUIProxy::print( char value ) const {
   char tmp[2] = { value, '\0' };
   return print( tmp );
}

size_t JavaUIProxy::print( uint8_t value, int base ) const {
   return print((long)value, base );
}

size_t JavaUIProxy::print( int value, int base ) const {
   return print((long)value, base );
}

size_t JavaUIProxy::print( unsigned int value, int base ) const {
   return print((long)value, base );
}

size_t JavaUIProxy::print( long value, int base ) const {
   return print((long)value, base );
}

size_t JavaUIProxy::print( unsigned long value, int base ) const {
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
   const char * str = ss.str().c_str();
   if( _serialTeeStderr ) {
      ::fprintf( stderr, "%s", str );
   }
   return send( E_PRINT, str );
}

size_t JavaUIProxy::print( double value, int prec ) const {
   std::stringstream ss;
   ss << std::setprecision( prec ) << value;
   const char * str = ss.str().c_str();
   if( _serialTeeStderr ) {
      ::fprintf( stderr, "%s", str );
   }
   return send( E_PRINT, str );
}

size_t JavaUIProxy::println( const char value[] ) const {
   return send( E_PRINTLN, value );
}

size_t JavaUIProxy::println( char value ) const {
   char tmp[2] = { value, '\0' };
   return println( tmp );
}

size_t JavaUIProxy::println( uint8_t value, int base ) const {
   return println((long)value, base );
}

size_t JavaUIProxy::println( int value, int base ) const {
   return println((long)value, base );
}

size_t JavaUIProxy::println( unsigned int value, int base ) const {
   return println((long)value, base );
}

size_t JavaUIProxy::println( long value, int base ) const {
   return println((long)value, base );
}

size_t JavaUIProxy::println( unsigned long value, int base ) const {
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
   const char * str = ss.str().c_str();
   if( _serialTeeStderr ) {
      ::fprintf( stderr, "%s", str );
   }
   return send( E_PRINTLN, str );
}

size_t JavaUIProxy::println( double value, int prec ) const {
   std::stringstream ss;
   ss << std::setprecision( prec ) << value;
   const char * str = ss.str().c_str();
   if( _serialTeeStderr ) {
      ::fprintf( stderr, "%s", str );
   }
   return send( E_PRINTLN, str );
}

size_t JavaUIProxy::println( void ) const {
   return println( "" );
}

//-- Servo -------------------------------------------------------------------

void JavaUIProxy::servoAttach( uint8_t pin ) const {
   char   buffer[1+1];
   char * p = buffer;
   *p = E_SERVO_ATTACH;
   p += 1;
   *((uint8_t*)p) = pin;
   send( buffer, sizeof( buffer ));
}

void JavaUIProxy::servoWrite( uint8_t pin, int value ) const {
   char   buffer[1+1+4];
   char * p = buffer;
   *p = E_SERVO_WRITE;
   p += 1;
   *((uint8_t*)p) = pin;
   p += 1;
   *((int*)p) = ntohl( value );
   send( buffer, sizeof( buffer ));
}

void JavaUIProxy::servoDetach( uint8_t pin ) const {
   char   buffer[1+1];
   char * p = buffer;
   *p = E_SERVO_DETACH;
   p += 1;
   *((uint8_t*)p) = pin;
   send( buffer, sizeof( buffer ));
}

void JavaUIProxy::_recv() const {
   for(;;) {
      char buffer[1024];
      socklen_t len = sizeof( a->addr );
      int count = ::recvfrom( a->socket, buffer, sizeof( buffer ), MSG_WAITALL,
         (struct sockaddr *)&a->addr, &len );
      if( count < 0 ) {
         perror("recvfrom()");
         ::exit( EXIT_FAILURE );
      }
      verb_t verb = (verb_t)buffer[0];
      if( verb == E_RESET ) {
         a -> status = sim::E_RESET;
      }
      else if( verb == E_DIGITAL_READ ) {
         int pin    = buffer[1];
         int status = buffer[2];
         a->digital[pin] = status;
         if(( pin == 2 )
            &&(     a->isr_0_func )
            &&(  (( a->isr_0_mode == RISING  ) &&  status )
               ||(( a->isr_0_mode == FALLING ) && !status )
               ||(  a->isr_0_mode == CHANGE               )))
         {
            a->isr_0_func();
         }
         else if(
            ( pin == 3 )
            &&(     a->isr_1_func )
            &&(  (( a->isr_1_mode == RISING  ) &&  status )
               ||(( a->isr_1_mode == FALLING ) && !status )
               ||(  a->isr_1_mode == CHANGE               )))
         {
            a->isr_1_func();
         }
      }
      else if( verb == E_EXIT ) {
         int exitCode = ntohl( *(int*)(buffer+1));
         ::exit( exitCode );
      }
   }
}

size_t JavaUIProxy::send( int verb, const char * s ) const {
   if( ! s ) {
      s = "(null)";
   }
   unsigned short len = (unsigned short)strlen(s);
   char buffer[1+2+len];
   buffer[0] = verb;
   *((unsigned short *)(buffer+1)) = htons( len );
   strncpy( buffer+3, s, len );
   return send( buffer, sizeof( buffer ));
}

size_t JavaUIProxy::send( const char * buffer, unsigned short len ) const {
   int bytesSent = ::sendto( a->socket, buffer, len, 0, (const sockaddr*)&a->addr, sizeof( a->addr ));
   if( bytesSent < 0 ) {
      perror("sendto()");
      ::exit( EXIT_FAILURE );
   }
   return bytesSent;
}
