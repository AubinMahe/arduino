#include "Encoder.h"

using namespace json;

Encoder::Encoder( char * buffer, size_t size ) :
   _buffer( buffer ),
   _size( size ),
   _end( buffer + _size ),
   _work( buffer )
{
   memset( _buffer, 0, _size );
}

Status Encoder::encode( const IJSonData & data ) {
   return data.getCoDec().encode( data, *this );
}

Status Encoder::openObject() {
   return append( "{", 1 );
}

Status Encoder::closeObject() {
   return append( "}", 1 );
}

Status Encoder::openArray() {
   return append( "[", 1 );
}

Status Encoder::closeArray() {
   return append( "]", 1 );
}

Status Encoder::property( const char * name, bool value ) {
   Status retVal = beginProperty( name );
   if( retVal == SUCCESS ) {
      retVal = append( value );
   }
   return retVal;
}

Status Encoder::property( const char * name, long value ) {
   Status retVal = beginProperty( name );
   if( retVal == SUCCESS ) {
      retVal = append( value );
   }
   return retVal;
}

Status Encoder::property( const char * name, double value ) {
   Status retVal = beginProperty( name );
   if( retVal == SUCCESS ) {
      retVal = append( value );
   }
   return retVal;
}

Status Encoder::property( const char * name, const char * value ) {
   Status retVal = beginProperty( name );
   if( retVal == SUCCESS ) {
      retVal = append( value);
   }
   return retVal;
}

Status Encoder::property( const char * name, const IJSonData & value ) {
   Status retVal = beginProperty( name );
   if( retVal == SUCCESS ) {
      retVal = encode( value );
   }
   return retVal;
}

Status Encoder::property( const char * name, const IJSonData * value, size_t size, size_t itemSize ) {
   Status retVal = beginProperty( name );
   if( retVal == SUCCESS ) {
      Status retVal = openArray();
      for( size_t i = 0; retVal == SUCCESS && i < size; ++i ) {
         if( i > 0 ) {
            retVal = separator();
         }
         if( retVal == SUCCESS ) {
            char * raw = (char *)value;
            raw += i*itemSize;
            IJSonData * item = (IJSonData *)raw;
            retVal = encode( *item );
         }
      }
      if( retVal == SUCCESS ) {
         retVal = closeArray();
      }
   }
   return retVal;
}

Status Encoder::property( const char * name, const char * value, size_t capacity, size_t size ) {
   Status retVal = beginProperty( name );
   if( retVal == SUCCESS ) {
      Status retVal = openArray();
      for( size_t i = 0; retVal == SUCCESS && i < size; ++i ) {
         if( i > 0 ) {
            retVal = separator();
         }
         if( retVal == SUCCESS ) {
            retVal = append( value );
            value += capacity;
         }
      }
      if( retVal == SUCCESS ) {
         retVal = closeArray();
      }
   }
   return retVal;
}

Status Encoder::separator( void ) {
   return append( ",", 1 );
}

Status Encoder::beginProperty( const char * name ) {
   Status retVal = append( "\"", 1 );
   if( retVal == SUCCESS ) {
      retVal = append( name, ::strlen( name ));
   }
   if( retVal == SUCCESS ) {
      retVal = append( "\":", 2 );
   }
   return retVal;
}

Status Encoder::append( bool value ) {
   return value ? append( "true", 4 ) : append( "false", 5 );
}

Status Encoder::append( unsigned char value ) {
   return append((long)value );
}

Status Encoder::append( short value ) {
   return append((long)value );
}

Status Encoder::append( int value ) {
   return append((long)value );
}

Status Encoder::append( long value ) {
   snprintf( _tmp, sizeof( _tmp ), "%ld", value );
   return append( _tmp, ::strlen( _tmp ));
}

Status Encoder::append( float value ) {
   return append((double)value );
}

Status Encoder::append( double value ) {
   snprintf( _tmp, sizeof( _tmp ), "%G", value );
   return append( _tmp, ::strlen( _tmp ));
}

Status Encoder::append( const char * value ) {
   Status retVal = append( "\"", 1 );
   if( retVal == SUCCESS ) {
      retVal = append( value, ::strlen( value ));
   }
   if( retVal == SUCCESS ) {
      retVal = append( "\"", 1 );
   }
   return retVal;
}

Status Encoder::append( const char * string, size_t len ) {
   if( _work + len >= _end ) {
      return BUFFER_OVERFLOW;
   }
   ::strncat( _buffer, string, len );
   _work += len;
   return SUCCESS;
}
