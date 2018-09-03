#pragma once

#include <string.h>
#include <stdlib.h>

#include <type_traits>

#include "IJSonData.h"

#ifndef MIN
#  define MIN(a,b) (( (a) < (b) ) ? (a) : (b))
#endif

namespace json {

   class Decoder {
   public:

      Decoder( const char * begin, size_t len = 0 ) :
         _begin( begin ),
         _end( 0 ),
         _work( _begin ),
         _decodedValueType( NONE ),
         _boolean( false ),
         _long( 0L ),
         _double( 0.0 )
      {
         memset( _string, 0, sizeof( _string ));
         if( len == 0 ) {
            _end = _begin + ::strlen( _begin );
         }
         else {
            _end = _begin + len;
         }
      }

      /**
       * On decode la chaine entre { et } et pour chaque attribut on cherche
       * son accesseur dans la liste pour initialiser l'objet avec la valeur
       * décodée.
       */
      template<class T, typename std::enable_if<std::is_base_of<IJSonData, T>::value>::type* = nullptr>
      Status decode( T & target ) {
         int c = skip_spaces();
         if( c == '{' ) {
            ++_work;
            while(( _work < _end )&&( c != '}' )) {
               c = skip_spaces();
               if( c == ',' ) {
                  ++_work;
                  c = skip_spaces();
               }
               if(( c == '\'' )||( c == '"' )) {
                  char name[100];
                  Status status = get_string( name, sizeof( name ));
                  if( status ) {
                     return status;
                  }
                  c = skip_spaces();
                  if( c == ':' ) {
                     ++_work;
                     c = skip_spaces();
                     if(( c == '\'' )||( c == '"' )) {
                        status = get_string( _string, sizeof( _string ));
                        if( status ) {
                           return status;
                        }
                     }
                     else if( c == '{' ) {
                     }
                     else if( c == '[' ) {
                     }
                     else {
                        status = get_value();
                        if( status ) {
                           return status;
                        }
                     }
                     status = target.decode( name, *this );
                     if( status ) {
                        return status;
                     }
                  }
                  c = skip_spaces();
               }
               else {
                  return BEGIN_OF_STRING_NOT_FOUND;
               }
            }
         }
         else {
            return OPEN_BRACE_EXPECTED;
         }
         if( _work < _end ) {
            ++_work;
         }
         return SUCCESS;
      }

      template<class T>
      Status decode( T & array, size_t card ) {
         ++_work;
         int c = skip_spaces();
         for( size_t i = 0; i < card; ++i ) {
            Status status = decode( array[i] );
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

      const char * getError() const {
         return _work;
      }

      Status get( bool & target ) const {
         if( _decodedValueType != BOOLEAN ) {
            return TYPE_MISMATCH;
         }
         target = _boolean;
         return SUCCESS;
      }

      Status get( unsigned char & target ) const {
         if( _decodedValueType != INTEGER ) {
            return TYPE_MISMATCH;
         }
         target = _long;
         return SUCCESS;
      }

      Status get( short & target ) const {
         if( _decodedValueType != INTEGER ) {
            return TYPE_MISMATCH;
         }
         target = _long;
         return SUCCESS;
      }

      Status get( unsigned short & target ) const {
         if( _decodedValueType != INTEGER ) {
            return TYPE_MISMATCH;
         }
         target = _long;
         return SUCCESS;
      }

      Status get( int & target ) const {
         if( _decodedValueType != INTEGER ) {
            return TYPE_MISMATCH;
         }
         target = _long;
         return SUCCESS;
      }

      Status get( unsigned int & target ) const {
         if( _decodedValueType != INTEGER ) {
            return TYPE_MISMATCH;
         }
         target = _long;
         return SUCCESS;
      }

      Status get( long & target ) const {
         if( _decodedValueType != INTEGER ) {
            return TYPE_MISMATCH;
         }
         target = _long;
         return SUCCESS;
      }

      Status get( unsigned long & target ) const {
         if( _decodedValueType != INTEGER ) {
            return TYPE_MISMATCH;
         }
         target = _long;
         return SUCCESS;
      }

      Status get( float & target ) const {
         if( _decodedValueType != DOUBLE ) {
            return TYPE_MISMATCH;
         }
         target = _double;
         return SUCCESS;
      }

      Status get( double & target ) const {
         if( _decodedValueType != DOUBLE ) {
            return TYPE_MISMATCH;
         }
         target = _double;
         return SUCCESS;
      }

      Status getString( char * target, size_t tgtSize ) const {
         if( _decodedValueType != STRING ) {
            return TYPE_MISMATCH;
         }
         strncpy( target, _string, tgtSize );
         target[tgtSize-1] = '\0';
         return SUCCESS;
      }

   private:

      int skip_spaces() {
         while(( _work < _end )&&( *_work == ' ' )) {
            ++_work;
         }
         return ( _work < _end ) ? *_work : -1;
      }

      Status get_string( char * target, size_t tgtSize ) {
         char eos = *_work;
         if(( eos != '\'' )&&( *_work != '"' )) {
            return BEGIN_OF_STRING_NOT_FOUND;
         }
         const char * begin = _work++;
         while(( _work < _end )&&(( *_work != eos )||( *(_work-1) == '\\' ))) {
            ++_work;
         }
         if( *_work != eos ) {
            return END_OF_STRING_NOT_FOUND;
         }
         size_t len = MIN( tgtSize - 1, (size_t)( _work - begin - 1 ));
         strncpy( target, begin + 1, len );
         target[len] = '\0';
         _work++;
         _decodedValueType = STRING;
         return SUCCESS;
      }

      Status get_value() {
         const char * begin = _work;
         while(( _work < _end )&&(( *_work != ',' )&&( *_work != '}' )&&( *_work != ' ' ))) {
            ++_work;
         }
         size_t len = MIN( sizeof( _string ) - 1, (size_t)( _work - begin ));
         strncpy( _string, begin, len );
         _string[len] = '\0';
         bool is_true ( 0 == strcmp( _string, "true"  ));
         bool is_false( 0 == strcmp( _string, "false" ));
         if( is_true || is_false ) {
            _boolean          = is_true;
            _decodedValueType = BOOLEAN;
         }
         else {
            char * err = 0;
            _long             = ::strtol( _string, &err, 10 );
            _decodedValueType = INTEGER;
            if( *err ) {
               _double           = ::strtod( _string, &err );
               _decodedValueType = DOUBLE;
               if( *err ) {
                  _decodedValueType = NONE;
                  return ERROR_PARSING_VALUE;
               }
            }
         }
         return SUCCESS;
      }

   private:

      const char *  _begin;
      const char *  _end;
      const char *  _work;

      enum Type {

         NONE,

         BOOLEAN,
         INTEGER,
         DOUBLE,
         STRING
      };

      Type   _decodedValueType;
      bool   _boolean;
      long   _long;
      double _double;
      char   _string[1024];
   };
}
