#include "Decoder.h"
#include "IJSonData.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <type_traits>

using namespace json;

char Decoder:: _errMsg[200];

Decoder::Decoder( const char * begin, size_t len /* = 0 */) :
   _begin( begin ),
   _end(( len == 0 ) ? ( _begin + ::strlen( _begin )) : (_begin + len )),
   _work( _begin ),
   _decodedValueType( NONE ),
   _boolean( false ),
   _int( 0L ),
   _double( 0.0 )
{
   memset( _string, 0, sizeof( _string ));
}

void Decoder::reset() {
   _work             = _begin;
   _decodedValueType = NONE;
   _boolean          = false;
   _int              = 0L;
   _double           = 0.0;
   memset( _string, 0, sizeof( _string ));
}

Status Decoder::decode( IJSonData & target ) {
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
               status = target.getCoDec().decode( name, target, *this );
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

Status Decoder::decode( IJSonData * array, size_t card, size_t itemSize ) {
   Status retVal = OPEN_BRACKET_EXPECTED;
   int c = skip_spaces();
   if( c == '[' ) {
      ++_work;
      retVal = SUCCESS;
      for( size_t i = 0; (retVal == SUCCESS)&&(_work < _end)&&(c != ']')&&(i < card); ++i ) {
         c = skip_spaces();
         if( c == ',' ) {
            ++_work;
            c = skip_spaces();
         }
         IJSonData * item = (IJSonData *)(((char *)array)+i*itemSize);
         retVal = decode( *item );
         c = skip_spaces();
      }
      if( c == ']' ) {
         ++_work;
         retVal = SUCCESS;
      }
   }
   return retVal;
}

const char * Decoder::getError() const {
   return _work;
}

bool Decoder::dump( const char * buffer, size_t count, char * target, size_t targetSize ) {
   ::memset( target, 0, targetSize );
   char line [4+2+3*16+1];
   char ascii[16+1];
   ascii[16] = '\0';
   *target = '\0';
   for( size_t i = 0; ( i < count )&&( ::strlen( target ) < targetSize ); i += 16 ) {
      ::snprintf( line, sizeof( line ), "%04X: ", (unsigned int)i );
      for( size_t col = 0; col < 16; ++col ) {
         char   tmp[4];
         size_t ndx = i + col;
         if( ndx < count ) {
            unsigned char c = buffer[ndx];
            ::snprintf( tmp, sizeof( tmp ), "%02X ", c );
            ascii[col] = (( c > 31 )&&(c < 127 )) ? c : '.';
         }
         else {
            ::snprintf( tmp, sizeof( tmp ), "   " );
            ascii[col] = ' ';
         }
         ::strcat( line, tmp );
      }
      ::strncat( target, line , targetSize );
      if( ::strlen( target ) < targetSize ) {
         ::strncat( target, "- " , targetSize );
      }
      if( ::strlen( target ) < targetSize ) {
         ::strncat( target, ascii, targetSize );
      }
      if( ::strlen( target ) < targetSize ) {
         ::strncat( target, "\n" , targetSize );
      }
   }
   return ::strlen( target ) < targetSize;
}

Status Decoder::decode( bool & target ) const {
   if( _decodedValueType != BOOLEAN ) {
      return TYPE_MISMATCH;
   }
   target = _boolean;
   return SUCCESS;
}

Status Decoder::decode( unsigned char & target ) const {
   if( _decodedValueType != INTEGER ) {
      return TYPE_MISMATCH;
   }
   target = _int;
   return SUCCESS;
}

Status Decoder::decode( short & target ) const {
   if( _decodedValueType != INTEGER ) {
      return TYPE_MISMATCH;
   }
   target = _int;
   return SUCCESS;
}

Status Decoder::decode( int & target ) const {
   if( _decodedValueType != INTEGER ) {
      return TYPE_MISMATCH;
   }
   target = _int;
   return SUCCESS;
}

Status Decoder::decode( float & target ) const {
   if(( _decodedValueType != DOUBLE )&&( _decodedValueType != INTEGER )) {
      return TYPE_MISMATCH;
   }
   target = (float)_double;
   return SUCCESS;
}

Status Decoder::decode( double & target ) const {
   if(( _decodedValueType != DOUBLE )&&( _decodedValueType != INTEGER )) {
      return TYPE_MISMATCH;
   }
   target = _double;
   return SUCCESS;
}

Status Decoder::decode( size_t card, char * target ) const {
   if( _decodedValueType != STRING ) {
      return TYPE_MISMATCH;
   }
   strncpy( target, _string, card );
   target[card-1] = '\0';
   return SUCCESS;
}

Status Decoder::decode( char * target, size_t capacity, size_t card ) {
   Status retVal = OPEN_BRACKET_EXPECTED;
   int c = skip_spaces();
   if( c == '[' ) {
      ++_work;
      size_t i = 0;
      retVal = SUCCESS;
      char * s = target;
      while(( retVal == SUCCESS )&&( _work < _end )&&( c != ']' )&&( i < card )) {
         c = skip_spaces();
         if( c == ',' ) {
            ++_work;
            c = skip_spaces();
         }
         retVal = get_string( s, capacity );
         if( retVal == SUCCESS ) {
            s += capacity;
            c = skip_spaces();
         }
      }
      if( c == ']' ) {
         ++_work;
         retVal = SUCCESS;
      }
   }
   return retVal;
}

int Decoder::skip_spaces() {
   while(( _work < _end )&&( *_work == ' ' )) {
      ++_work;
   }
   return ( _work < _end ) ? *_work : -1;
}

Status Decoder::get_string( char * target, size_t size ) {
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
   size_t len = MIN( size - 1, (size_t)( _work - begin - 1 ));
   strncpy( target, begin + 1, len );
   target[len] = '\0';
   _work++;
   _decodedValueType = STRING;
   return SUCCESS;
}

Status Decoder::get_value() {
   const char * begin = _work;
   while(( _work < _end )
      &&( *_work != ',' )
      &&( *_work != '}' )
      &&( *_work != ']' )
      &&( *_work != ' ' ))
   {
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
      _int             = ::strtol( _string, &err, 10 );
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
