#pragma once

#include <string.h>
#include <stdlib.h>

#include <type_traits>

#include "Attributes.h"

#ifndef MIN
#  define MIN(a,b) (( (a) < (b) ) ? (a) : (b))
#endif

namespace json {

   class Decoder {
   public:

      Decoder( const char * begin, size_t len = 0 );

      Status decode( IJSonData & target );
      Status decode( IJSonData * array, size_t card, size_t itemSize );

      template<typename T, size_t C, typename std::enable_if<std::is_base_of<IJSonData, T>::value>::type* = nullptr>
      Status decode( T array[C] ) {
         return decode( array, C, sizeof( array[0] ));
      }

      template<class T, typename std::enable_if<std::is_scalar<T>::value>::type* = nullptr>
      Status decode( T & array, size_t card ) {
         ++_work;
         int c = skip_spaces();
         for( size_t i = 0; i < card; ++i ) {
            c = skip_spaces();
            Status status;
            if(( c == '\'' )||( c == '"' )) {
               status = get_string( _string, sizeof( _string ));
            }
            else if( c == '{' ) {
            }
            else if( c == '[' ) {
            }
            else {
               status = get_value();
            }
            if( status ) {
               return status;
            }
            status = decode( array[i] );
            if( SUCCESS != status ) {
               return status;
            }
            c = skip_spaces();
            if( c == ',' ) {
               ++_work;
               c = skip_spaces();
            }
            if( c == ']' ) {
               ++_work;
               break;
            }
         }
         return ( c == ']' ) ? SUCCESS : CLOSE_BRACKET_EXPECTED;
      }

      Status decode( bool &              target ) const;
      Status decode( unsigned char &     target ) const;
      Status decode( short &             target ) const;
      Status decode( int &               target ) const;
      Status decode( float &             target ) const;
      Status decode( double &            target ) const;
      Status decode( size_t card, char * target ) const;

      const char * getError() const;

   private:

      int skip_spaces();

      Status get_string( char * target, size_t tgtSize );
      Status get_value();

   private:

      const char * _begin;
      const char * _end;
      const char * _work;

      enum Type {

         NONE,

         BOOLEAN,
         INTEGER,
         DOUBLE,
         STRING
      };

      Type   _decodedValueType;
      bool   _boolean;
      int    _int;
      double _double;
      char   _string[1024];
   };
}
