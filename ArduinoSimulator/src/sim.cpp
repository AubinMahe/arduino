#include <Arduino.h>

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

enum verb_t {

   E_NONE,

   //-- Digital I/O -------------------------------------------------------------

   E_DIGITAL_READ,
   E_DIGITAL_WRITE,
   E_PIN_MODE,

   //-- Analog I/O --------------------------------------------------------------

   E_ANALOG_READ,
   E_ANALOG_REFERENCE,
   E_ANALOG_WRITE,

   //-- Advanced I/O ------------------------------------------------------------

   E_NO_TONE,
   E_TONE,

   //-- Communication -----------------------------------------------------------

   E_PRINT,
   E_PRINTLN,

   E_EXIT
};

class UI {
public:

   UI( unsigned short port ) {
      memset( _digital, 0, sizeof( _digital ));
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
      if( ::pthread_create( &_thread, 0, UI::recv, this )) {
         perror("pthread_create()");
         ::exit( EXIT_FAILURE );
      }
   }

public:

   //-- Digital I/O -------------------------------------------------------------

   int digitalRead ( uint8_t pin ) {
      return _digital[pin];
   }

   void digitalWrite( uint8_t pin, uint8_t hiOrLo ) {
      char buffer[1+1+1];
      char * p = buffer;
      *p = E_DIGITAL_WRITE;
      p += 1;
      *((uint8_t*)p) = pin;
      p += 1;
      *((uint8_t*)p) = hiOrLo;
      send( buffer, sizeof( buffer ));
   }

   void pinMode( uint8_t pin, uint8_t inOrOut ) {
      char buffer[1+1+1];
      char * p = buffer;
      *p = E_PIN_MODE;
      p += 1;
      *((uint8_t*)p) = pin;
      p += 1;
      *((uint8_t*)p) = inOrOut;
      send( buffer, sizeof( buffer ));
   }

   //-- Analog I/O --------------------------------------------------------------

   int analogRead( uint8_t pin ) {
      return _analog[pin];
   }

   void analogReference( uint8_t mode ) {
      char buffer[1+1];
      char * p = buffer;
      *p = E_ANALOG_REFERENCE;
      p += 1;
      *((uint8_t*)p) = mode;
      send( buffer, sizeof( buffer ));
   }

   void analogWrite( uint8_t pin, int value ) {
      char buffer[1+1+4];
      char * p = buffer;
      *p = E_ANALOG_WRITE;
      p += 1;
      *((uint8_t*)p) = pin;
      p += 1;
      *((int*)p) = value;
      send( buffer, sizeof( buffer ));
   }

   //-- Advanced I/O ------------------------------------------------------------

   void noTone( uint8_t pin ) {
      char buffer[1+1];
      char * p = buffer;
      *p = E_NO_TONE;
      p += 1;
      *((uint8_t*)p) = pin;
      send( buffer, sizeof( buffer ));
   }

   void tone( uint8_t pin, unsigned int frequency, unsigned long duration ) {
      char buffer[1+1+4+4];
      char * p = buffer;
      *p = E_TONE;
      p += 1;
      *((uint8_t*)p) = pin;
      p += 1;
      *((unsigned int *)p) = htonl( frequency );
      p += sizeof( int );
      *((unsigned int *)p) = htonl( duration );
      send( buffer, sizeof( buffer ));
   }

   //-- Communication -----------------------------------------------------------

   void print( const char * s ) {
      send( E_PRINT, s );
   }

   void println( const char * s ) {
      send( E_PRINTLN, s );
   }

private:

   static void * recv( void * arg ) {
      UI * ui = (UI *)arg;
      for(;;) {
         char buffer[1024];
         socklen_t len = sizeof( ui->_addr );
         int count = ::recvfrom(
            ui->_socket, buffer, sizeof( buffer ), MSG_WAITALL,
            (struct sockaddr *)&ui->_addr, &len );
         if( count < 0 ) {
            perror("recvfrom()");
            ::exit( EXIT_FAILURE );
         }
         verb_t verb = (verb_t)buffer[0];
         if( verb == E_DIGITAL_READ ) {
            int pin    = buffer[1];
            int status = buffer[2];
            ui->_digital[pin] = status;
         }
         else if( verb == E_EXIT ) {
            int exitCode = ntohl( *(int*)(buffer+1));
            ::exit( exitCode );
         }
      }
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
};

UI ui( 2416 );

//-- Digital I/O -------------------------------------------------------------

int digitalRead ( uint8_t pin ) {
   return ui.digitalRead( pin );
}

void digitalWrite( uint8_t pin, uint8_t hiOrLo ) {
   ui.digitalWrite( pin, hiOrLo );
}

void pinMode( uint8_t pin, uint8_t inOrOut ) {
   ui.pinMode( pin, inOrOut );
}

//-- Analog I/O --------------------------------------------------------------

int analogRead( uint8_t pin ) {
   return ui.analogRead( pin );
}

void analogReference( uint8_t mode) {
   ui.analogReference( mode );
}

void analogWrite( uint8_t pin, int value ) {
   ui.analogWrite( pin, value );
}

//-- Advanced I/O ------------------------------------------------------------

void noTone( uint8_t pin ) {
   ui.noTone( pin );
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
   ui.tone( pin, frequency, duration );
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
   // TODO
}

void detachInterrupt( uint8_t interrupt ) {
   // TODO
}

//-- Interrupts --------------------------------------------------------------

//-- Communication -----------------------------------------------------------

void Print::print( const char * s ) {
   ui.print( s );
}

void Print::println( const char * s ) {
   ui.println( s );
}

void HardwareSerial::begin( unsigned long, uint8_t ) {
}

HardwareSerial Serial;

//-- Main --------------------------------------------------------------------

int main( int argc, char * argv[] ) {
   setup();
   for(;;) {
      loop();
   }
   return 0;
}
