#pragma once

#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "Status.h"

namespace json {

   class Generator {
   public:

      Generator( char * buffer, size_t size ) :
         _buffer( buffer ),
         _size( size ),
         _end( buffer + _size ),
         _work( buffer )
      {
         memset( _buffer, 0, _size );
      }

      template<class T>
      Status encode( const T & t ) {
         assert( _work[0] == '\0' );
         if( _work + 1 > _end ) {
            return BUFFER_OVERFLOW;
         }
         strcat( _buffer, "{" );
         ++_work;
         Status status = t.encode( *this );
         if( status != SUCCESS ) {
            return status;
         }
         if( _work + 1 > _end ) {
            return BUFFER_OVERFLOW;
         }
         strcat( _buffer, "}" );
         ++_work;
         assert( _work[0] == '\0' );
         return SUCCESS;
      }

      template<class T>
      Status encode( const char * name, T t ) {
         assert( _work[0] == '\0' );
         const char * value = format( t );
         bool first = ( _work[-1] == '{' );
         size_t len = ( first ? 0 : 1 ) + 1 + strlen( name ) + 2 + strlen( value );
         if( _work < _end + len ) {
            if( ! first ) {
               strcat( _buffer, "," );
            }
            strcat( _buffer, "\"" );
            strcat( _buffer, name );
            strcat( _buffer, "\":" );
            strcat( _buffer, value );
            _work += len;
            assert( _work[0] == '\0' );
            return SUCCESS;
         }
         return BUFFER_OVERFLOW;
      }

      template<class T>
      Status encodeObject( const char * name, const T & value ) {
         assert( _work[0] == '\0' );
         bool first = ( _work[-1] == '{' );
         size_t len = ( first ? 0 : 1 ) + 1 + strlen( name ) + 2;
         if( _work + len >= _end ) {
            return BUFFER_OVERFLOW;
         }
         if( ! first ) {
            strcat( _buffer, "," );
         }
         strcat( _buffer, "\"" );
         strcat( _buffer, name );
         strcat( _buffer, "\":" );
         _work += len;
         return encode( value );
      }

      template<class T>
      Status encodeArray( const char * name, T value[], size_t size ) {
         assert( _work[0] == '\0' );
         bool first = ( _work[-1] == '{' );
         size_t len = ( first ? 0 : 1 ) + 1 + strlen( name ) + 3;
         if( _work + len >= _end ) {
            return BUFFER_OVERFLOW;
         }
         if( ! first ) {
            strcat( _buffer, "," );
         }
         strcat( _buffer, "\"" );
         strcat( _buffer, name );
         strcat( _buffer, "\":[" );
         _work += len;
         for( size_t i = 0; _work < _end && i < size; ++i ) {
            if( i ) {
               if( _work + 1 >= _end ) {
                  return BUFFER_OVERFLOW;
               }
               strcat( _buffer, "," );
               ++_work;
            }
            char * item = format( value[i] );
            strcat( _buffer, item );
            _work += strlen( item );
         }
         if( _work + 1 >= _end ) {
            return BUFFER_OVERFLOW;
         }
         strcat( _buffer, "]" );
         ++_work;
         assert( _work[0] == '\0' );
         return SUCCESS;
      }

      template<class T>
      Status encodeObjectArray( const char * name, const T value[], size_t size ) {
         assert( _work[0] == '\0' );
         bool first = ( _work[-1] == '{' );
         size_t len = ( first ? 0 : 1 ) + 1 + strlen( name ) + 3;
         if( _work + len >= _end ) {
            return BUFFER_OVERFLOW;
         }
         if( ! first ) {
            strcat( _buffer, "," );
         }
         strcat( _buffer, "\"" );
         strcat( _buffer, name );
         strcat( _buffer, "\":[" );
         _work += len;
         for( size_t i = 0; i < size; ++i ) {
            if( i ) {
               if( _work + 1 >= _end ) {
                  return BUFFER_OVERFLOW;
               }
               strcat( _buffer, "," );
               ++_work;
            }
            Status status = encode( value[i] );
            if( status ) {
               return status;
            }
         }
         if( _work + 1 >= _end ) {
            return BUFFER_OVERFLOW;
         }
         strcat( _buffer, "]" );
         ++_work;
         assert( _work[0] == '\0' );
         return SUCCESS;
      }

   private:

      const char * format( bool value ) {
         return value ? "true" : "false";
      }

      const char * format( unsigned char value ) {
         snprintf( _tmp, sizeof( _tmp ), "%d", value );
         return _tmp;
      }

      const char * format( short value ) {
         snprintf( _tmp, sizeof( _tmp ), "%d", value );
         return _tmp;
      }

      const char * format( int value ) {
         snprintf( _tmp, sizeof( _tmp ), "%d", value );
         return _tmp;
      }

      const char * format( float value ) {
         snprintf( _tmp, sizeof( _tmp ), "%G", value );
         return _tmp;
      }

      const char * format( double value ) {
         snprintf( _tmp, sizeof( _tmp ), "%G", value );
         return _tmp;
      }

   private:

      char * const _buffer;
      const size_t _size;
      char * const _end;
      char *       _work;
      char         _tmp[100];
   };
}
