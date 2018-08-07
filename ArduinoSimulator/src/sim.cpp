#include <Arduino.h>
#include <Servo.h>

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

#define UI_PROXY_PORT 2416

enum verb_t {

   E_NONE,

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

class UI_proxy {
public:

   UI_proxy( unsigned short port ) {
      memset( _digital, 0, sizeof( _digital ));
      _isr_0_func = 0;
      _isr_0_mode = 0;
      _isr_0_func = 0;
      _isr_1_mode = 0;
      _socket = ::socket( AF_INET, SOCK_DGRAM, 0 );
      if( _socket < 0 ) {
         perror("socket()");
         ::exit( EXIT_FAILURE );
      }
      ::memset( &_addr, 0, sizeof( _addr ));
      _addr.sin_family      = AF_INET;
      _addr.sin_addr.s_addr = htonl( INADDR_ANY );
      _addr.sin_port        = htons( port + 1 );
      int st = ::bind( _socket, (sockaddr *)&_addr, sizeof( _addr ));
      if( st ) {
         perror("bind()");
         ::exit( EXIT_FAILURE );
      }
      _addr.sin_port = htons( port );
      if( ::pthread_create( &_thread, 0, UI_proxy::recv, this )) {
         perror("pthread_create()");
         ::exit( EXIT_FAILURE );
      }
   }

public:

   //-- Digital I/O ----------------------------------------------------------

   int digitalRead( uint8_t pin ) {
      return _digital[pin];
   }

   void digitalWrite( uint8_t pin, uint8_t hiOrLo ) {
      char   buffer[1+1+1];
      char * p = buffer;
      *p = E_DIGITAL_WRITE;
      p += 1;
      *((uint8_t*)p) = pin;
      p += 1;
      *((uint8_t*)p) = hiOrLo;
      send( buffer, sizeof( buffer ));
   }

   void pinMode( uint8_t pin, uint8_t inOrOut ) {
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

   int analogRead( uint8_t pin ) {
      return _analog[pin];
   }

   void analogReference( uint8_t mode ) {
      char   buffer[1+1];
      char * p = buffer;
      *p = E_ANALOG_REFERENCE;
      p += 1;
      *((uint8_t*)p) = mode;
      send( buffer, sizeof( buffer ));
   }

   void analogWrite( uint8_t pin, int value ) {
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

   void noTone( uint8_t pin ) {
      char   buffer[1+1];
      char * p = buffer;
      *p = E_NO_TONE;
      p += 1;
      *((uint8_t*)p) = pin;
      send( buffer, sizeof( buffer ));
   }

   void tone( uint8_t pin, unsigned int frequency, unsigned long duration ) {
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

   void attachInterrupt( uint8_t pin, void (* ISR )(void), uint8_t mode ) {
      if( pin == 2 ) {
         _isr_0_func = ISR;
         _isr_0_mode = mode;
      }
      else {
         _isr_1_func = ISR;
         _isr_1_mode = mode;
      }
   }

   void detachInterrupt( uint8_t pin ) {
      if( pin == 2 ) {
         _isr_0_func = 0;
         _isr_0_mode = 0;
      }
      else {
         _isr_1_func = 0;
         _isr_1_mode = 0;
      }
   }

   //-- Communication --------------------------------------------------------

   void print( const char * s ) {
      send( E_PRINT, s );
   }

   void println( const char * s ) {
      send( E_PRINTLN, s );
   }

   //-- Servo -------------------------------------------------------------------

   void servoAttach( uint8_t pin ) {
      char   buffer[1+1];
      char * p = buffer;
      *p = E_SERVO_ATTACH;
      p += 1;
      *((uint8_t*)p) = pin;
      send( buffer, sizeof( buffer ));
   }

   void servoWrite( uint8_t pin, int value ) {
      char   buffer[1+1+4];
      char * p = buffer;
      *p = E_SERVO_WRITE;
      p += 1;
      *((uint8_t*)p) = pin;
      p += 1;
      *((int*)p) = ntohl( value );
      send( buffer, sizeof( buffer ));
   }

   void servoDetach( uint8_t pin ) {
      char   buffer[1+1];
      char * p = buffer;
      *p = E_SERVO_DETACH;
      p += 1;
      *((uint8_t*)p) = pin;
      send( buffer, sizeof( buffer ));
   }

private:

   void _recv() {
      for(;;) {
         char buffer[1024];
         socklen_t len = sizeof( _addr );
         int count = ::recvfrom( _socket, buffer, sizeof( buffer ), MSG_WAITALL,
            (struct sockaddr *)&_addr, &len );
         if( count < 0 ) {
            perror("recvfrom()");
            ::exit( EXIT_FAILURE );
         }
         verb_t verb = (verb_t)buffer[0];
         if( verb == E_DIGITAL_READ ) {
            int pin    = buffer[1];
            int status = buffer[2];
            _digital[pin] = status;
            if(  ( pin == 2 )
               &&( _isr_0_func )
               &&(  (( _isr_0_mode == RISING  ) &&  status )
                  ||(( _isr_0_mode == FALLING ) && !status )))
            {
               _isr_0_func();
            }
            else if(  ( pin == 3 )
                    &&( _isr_1_func )
                    &&(  (( _isr_1_mode == RISING  ) &&  status )
                       ||(( _isr_1_mode == FALLING ) && !status )))
            {
               _isr_1_func();
            }
         }
         else if( verb == E_EXIT ) {
            int exitCode = ntohl( *(int*)(buffer+1));
            ::exit( exitCode );
         }
      }
   }

   static void * recv( void * arg ) {
      ((UI_proxy *)arg) -> _recv();
      return 0;
   }

   void send( verb_t verb, const char * s ) {
      if( ! s ) {
         s = "(null)";
      }
      unsigned short len = (unsigned short)strlen(s);
      char buffer[1+2+len];
      buffer[0] = verb;
      *((unsigned short *)(buffer+1)) = htons( len );
      strncpy( buffer+3, s, len );
      send( buffer, sizeof( buffer ));
   }

   void send( const char * buffer, unsigned short len ) {
      int bytesSent = ::sendto( _socket, buffer, len, 0, (const sockaddr*)&_addr, sizeof( _addr ));
      if( bytesSent < 0 ) {
         perror("sendto()");
         ::exit( EXIT_FAILURE );
      }
   }

private:

   int         _socket;
   sockaddr_in _addr;
   pthread_t   _thread;
   uint8_t     _digital[16];
   int         _analog [16];
   void    ( * _isr_0_func )( void );
   uint8_t     _isr_0_mode;
   void    ( * _isr_1_func )( void );
   uint8_t     _isr_1_mode;
};

UI_proxy proxy( UI_PROXY_PORT );

//-- Digital I/O -------------------------------------------------------------

int digitalRead ( uint8_t pin ) {
   return proxy.digitalRead( pin );
}

void digitalWrite( uint8_t pin, uint8_t hiOrLo ) {
   proxy.digitalWrite( pin, hiOrLo );
}

void pinMode( uint8_t pin, uint8_t inOrOut ) {
   proxy.pinMode( pin, inOrOut );
}

//-- Analog I/O --------------------------------------------------------------

int analogRead( uint8_t pin ) {
   return proxy.analogRead( pin );
}

void analogReference( uint8_t mode) {
   proxy.analogReference( mode );
}

void analogWrite( uint8_t pin, int value ) {
   proxy.analogWrite( pin, value );
}

//-- Advanced I/O ------------------------------------------------------------

void noTone( uint8_t pin ) {
   proxy.noTone( pin );
}

unsigned long pulseIn( uint8_t pin, uint8_t state, unsigned long timeout ) {
   // TODO
}

unsigned long pulseInLong( uint8_t pin, uint8_t state, unsigned long timeout ) {
   // TODO
}

uint8_t shiftIn( uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder ) {
   // TODO
   return 0;
}

void shiftOut( uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val ) {
   // TODO
}

void tone( uint8_t pin, unsigned int frequency, unsigned long duration ) {
   proxy.tone( pin, frequency, duration );
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
      proxy.attachInterrupt( 2, ISR, mode );
   }
   else if( interrupt == 1 ) {
      proxy.attachInterrupt( 3, ISR, mode );
   }
}

void detachInterrupt( uint8_t interrupt ) {
   if( interrupt == 0 ) {
      proxy.detachInterrupt( 2 );
   }
   else if( interrupt == 1 ) {
      proxy.detachInterrupt( 3 );
   }
}

//-- Interrupts --------------------------------------------------------------

//-- Communication -----------------------------------------------------------

void Print::print( const char * s ) {
   proxy.print( s );
}

void Print::println( const char * s ) {
   proxy.println( s );
}

void HardwareSerial::begin( unsigned long, uint8_t ) {
}

HardwareSerial Serial;

//-- Servo -------------------------------------------------------------------

Servo::Servo() {
   _pin = -1;
}

uint8_t Servo::attach( int pin ) {
   proxy.servoAttach( pin );
   _pin = pin;
   return 1;
}

uint8_t Servo::attach( int pin, int min, int max ) {
   return attach( pin );
}

void Servo::detach() {
   proxy.servoDetach( _pin );
   _pin = -1;
}

void Servo::write( int value ) {
   proxy.servoWrite( _pin, value );
}

void Servo::writeMicroseconds( int value ) {
}

int Servo::read() {
   return 0;
}

int Servo::readMicroseconds() {
   return 0;
}

bool Servo::attached() {
   return _pin != -1;
}

//-- Main --------------------------------------------------------------------

int main( int argc, char * argv[] ) {
   timespec spec;
   setup();
   for(;;) {
      loop();
      spec.tv_sec  = 0;
      spec.tv_nsec = 20 * 1000 * 1000; // 20 ms
      nanosleep( &spec, 0 );
   }
   return 0;
}
