#include "Encoder.h"
#include "Decoder.h"

#include <string.h>

typedef unsigned char byte;

struct AllScalar : public json::IJSonData {

   AllScalar() :
      _bool  ( true ),
      _byte  ( 1 ),
      _short ( 2 ),
      _int   ( 3 ),
      _float ( 4.5f ),
      _double( 6.7 )
   {
      strcpy( _string, "6 - Une chaine - 6" );
   }

   virtual const json::CoDec & getCoDec() const;

   bool operator == ( const AllScalar & right ) const {
      return( _bool   == right._bool   )
         && ( _byte   == right._byte   )
         && ( _short  == right._short  )
         && ( _int    == right._byte   )
         && ( _float  == right._float  )
         && ( _double == right._double )
         && ( 0 == ::strncmp( _string, right._string, 40 ));
   }

   bool   _bool;
   byte   _byte;
   short  _short;
   int    _int;
   float  _float;
   double _double;
   char   _string[40];

   friend struct AllScalarCoDec;
};

struct ArrayOfScalar : public json::IJSonData {

   ArrayOfScalar() {
      _array[0] = 1.2;
      _array[1] = 3.4;
      _array[2] = 5.6;
      _array[3] = 7.8;
   }

   virtual const json::CoDec & getCoDec() const;

   bool operator == ( const ArrayOfScalar & right ) const {
      return( _array[0] == right._array[0] )
         && ( _array[1] == right._array[1] )
         && ( _array[2] == right._array[2] )
         && ( _array[3] == right._array[3] );
   }

   double _array[4];

   friend struct ArrayOfScalarCoDec;
};

struct ArrayOfObject : public json::IJSonData {

   virtual const json::CoDec & getCoDec() const;

   bool operator == ( const ArrayOfObject & right ) const {
      return( _array[0] == right._array[0] )
         && ( _array[1] == right._array[1] );
   }

   AllScalar _array[2];

   friend struct ArrayOfObjectCoDec;
};

struct Composite : public json::IJSonData {

   virtual const json::CoDec & getCoDec() const;

   bool operator == ( const Composite & right ) const {
      return( _arrayOfScalar == right._arrayOfScalar )
         && ( _allScalar     == right._allScalar     )
         && ( _arrayOfObject == right._arrayOfObject );
   }

   ArrayOfScalar _arrayOfScalar;
   AllScalar     _allScalar;
   ArrayOfObject _arrayOfObject;

   friend struct CompositeCoDec;
};

struct AllScalarCoDec : public json::CoDec {

   static AllScalarCoDec codec;

   AllScalarCoDec() :
      json::CoDec(
         new json::Boolean( "bool"  , &AllScalar::_bool,
         new json::Byte   ( "byte"  , &AllScalar::_byte,
         new json::Short  ( "short" , &AllScalar::_short,
         new json::Integer( "int"   , &AllScalar::_int,
         new json::Float  ( "float" , &AllScalar::_float,
         new json::Double ( "double", &AllScalar::_double,
         new json::String ( "string", &AllScalar::_string ))))))))
   {}

   void clear( void ) {
      delete _attributes;
      _attributes = 0;
   }
};

AllScalarCoDec AllScalarCoDec::codec;

const json::CoDec & AllScalar::getCoDec() const {
   return AllScalarCoDec::codec;
}

struct ArrayOfScalarCoDec : public json::CoDec {

   static ArrayOfScalarCoDec codec;

   ArrayOfScalarCoDec() :
      json::CoDec(
         new json::DoubleArray( "array", &ArrayOfScalar::_array ))
   {}

   void clear( void ) {
      delete _attributes;
      _attributes = 0;
   }
};

ArrayOfScalarCoDec ArrayOfScalarCoDec::codec;

const json::CoDec & ArrayOfScalar::getCoDec() const {
   return ArrayOfScalarCoDec::codec;
}

struct CompositeCoDec : public json::CoDec {

   static CompositeCoDec codec;

   CompositeCoDec() :
      json::CoDec(
         new json::Object( "arrayOfScalar", &Composite::_arrayOfScalar,
         new json::Object( "allScalar"    , &Composite::_allScalar,
         new json::Object( "arrayOfObject", &Composite::_arrayOfObject ))))
   {}

   void clear( void ) {
      delete _attributes;
      _attributes = 0;
   }
};

CompositeCoDec CompositeCoDec::codec;

const json::CoDec & Composite::getCoDec() const {
   return CompositeCoDec::codec;
}

struct ArrayOfObjectCoDec : public json::CoDec {

   static ArrayOfObjectCoDec codec;

   ArrayOfObjectCoDec() :
      json::CoDec(
         new json::ObjectArray( "array", &ArrayOfObject::_array ))
   {}

   void clear( void ) {
      delete _attributes;
      _attributes = 0;
   }
};

ArrayOfObjectCoDec ArrayOfObjectCoDec::codec;

const json::CoDec & ArrayOfObject::getCoDec() const {
   return ArrayOfObjectCoDec::codec;
}

static void encode( json::Status expected, size_t size ) {
   Composite object;
   char * buffer = new char[size];
   json::Encoder e( buffer, size );
   json::Status status = e.encode( object );
   if( status == expected ) {
      ::fprintf( stderr, "ENCODE(%zu): ok\n", size );
   }
   else {
      ::fprintf( stderr, "\33[31mENCODE(%zu): BAD\33[0m\n", size );
   }
   ::printf( "(%zu):(%s)\n", strlen( buffer ), buffer );
   delete [] buffer;
}

static void decode( void ) {
   Composite object;
   char buffer[1024];
   json::Encoder e( buffer, sizeof( buffer ));
   json::Status status = e.encode( object );
   if( status == json::SUCCESS ) {
      json::Decoder d( buffer );
      Composite empty;
      json::Status status = d.decode( empty );
      if( status == json::SUCCESS ) {
         ::fprintf( stderr, "DECODE: ok\n" );
      }
      else {
         if( object == empty ) {
            ::fprintf( stderr, "DECODE: ok\n" );
         }
         else {
            ::fprintf( stderr, "\33[31mDECODE: BAD : object not affected\33[0m\n" );
         }
      }
   }
}

int main( void ) {
   encode( json::SUCCESS        , 1024 );
   encode( json::SUCCESS        ,  374 );
   encode( json::BUFFER_OVERFLOW,  373 );
   decode();
   AllScalarCoDec    ::codec.clear();
   ArrayOfScalarCoDec::codec.clear();
   CompositeCoDec    ::codec.clear();
   return 0;
}
