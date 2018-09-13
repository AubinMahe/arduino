#pragma once

#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <type_traits>

#include "Attributes.h"

namespace json {

   class Encoder {
   public:

      Encoder( char * buffer, size_t size );

   public:

      Status encode( const IJSonData & data );

   private:

      Status openObject();
      Status closeObject();
      Status openArray();
      Status closeArray();
      Status property( const char * name, bool              value );
      Status property( const char * name, long              value );
      Status property( const char * name, double            value );
      Status property( const char * name, const char *      value );
      Status property( const char * name, const IJSonData & value );
      Status property( const char * name, const IJSonData * value, size_t size, size_t itemSize );

      template<class T, typename std::enable_if<std::is_scalar<T>::value>::type* = nullptr>
      Status property( const char * name, const T * value, size_t size ) {
         Status retVal = beginProperty( name );
         if( retVal == SUCCESS ) {
            Status retVal = openArray();
            for( size_t i = 0; retVal == SUCCESS && i < size; ++i ) {
               if( i > 0 ) {
                  retVal = separator();
               }
               if( retVal == SUCCESS ) {
                  auto & item = value[i];
                  retVal = append( item );
               }
            }
            if( retVal == SUCCESS ) {
               retVal = closeArray();
            }
         }
         return retVal;
      }
      Status property( const char * name, const char * value, size_t capacity, size_t size );

      Status separator( void );

      Status beginProperty( const char * name );

      Status append( bool          value );
      Status append( unsigned char value );
      Status append( short         value );
      Status append( int           value );
      Status append( long          value );
      Status append( float         value );
      Status append( double        value );
      Status append( const char *  value );
      Status append( const char *  string, size_t len );

   private:

      char * const _buffer;
      const size_t _size;
      char * const _end;
      char *       _work;
      char         _tmp[100];

   friend class CoDec;
   friend class Boolean;
   friend class Byte;
   friend class Short;
   friend class Int;
   friend class Long;
   friend class Float;
   friend class Double;
   friend class String;
   friend class BooleanArray;
   friend class ByteArray;
   friend class ShortArray;
   friend class IntArray;
   friend class LongArray;
   friend class FloatArray;
   friend class DoubleArray;
   friend class StringArray;
   friend class Object;
   friend class ObjectArray;
   };
}
