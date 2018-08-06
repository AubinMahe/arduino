#pragma once

#include <stdint.h>

class Servo {
public:

   Servo( void );

public:

   uint8_t attach( int pin );
   uint8_t attach( int pin, int min, int max );
   void    detach();
   void    write( int value );
   void    writeMicroseconds( int value );
   int     read();
   int     readMicroseconds();
   bool    attached();

private:

   uint8_t _pin;
};
