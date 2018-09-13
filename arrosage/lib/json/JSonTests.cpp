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

   void reset() {
      _bool      = 0;
      _byte      = 0;
      _short     = 0;
      _int       = 0;
      _float     = 0.0f;
      _double    = 0.0;
      _string[0] = '\0';
   }

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
};

struct ArrayOfScalar : public json::IJSonData {

   ArrayOfScalar() {
      _bool  [0] = true;
      _bool  [1] = false;
      _byte  [0] = 0;
      _byte  [1] = 1;
      _byte  [2] = 2;
      _short [0] = 0;
      _short [1] = 1;
      _short [2] = 2;
      _short [3] = 3;
      _int   [0] = 0;
      _int   [1] = 1;
      _int   [2] = 2;
      _int   [3] = 3;
      _int   [4] = 4;
      _float [0] = 0.0f;
      _float [1] = 1.1f;
      _float [2] = 2.2f;
      _float [3] = 3.3f;
      _float [4] = 4.4f;
      _float [5] = 5.5f;
      _double[0] = 1.1;
      _double[1] = 2.2;
      _double[2] = 3.3;
      _double[3] = 4.4;
      _double[4] = 5.5;
      _double[5] = 6.6;
      _double[6] = 7.7;
      ::strcpy( _string[0], "1234567890123456789012345678901234567890" );
      ::strcpy( _string[1], "2345678901234567890123456789012345678901" );
      ::strcpy( _string[2], "3456789012345678901234567890123456789012" );
      ::strcpy( _string[3], "4567890123456789012345678901234567890123" );
      ::strcpy( _string[4], "5678901234567890123456789012345678901234" );
      ::strcpy( _string[5], "6789012345678901234567890123456789012345" );
      ::strcpy( _string[6], "7890123456789012345678901234567890123456" );
      ::strcpy( _string[7], "8901234567890123456789012345678901234567" );
   }

   virtual const json::CoDec & getCoDec() const;

   void reset() {
      _bool  [0] = false;
      _bool  [1] = false;
      _byte  [0] = 0;
      _byte  [1] = 0;
      _byte  [2] = 0;
      _short [0] = 0;
      _short [1] = 0;
      _short [2] = 0;
      _short [3] = 0;
      _int   [0] = 0;
      _int   [1] = 0;
      _int   [2] = 0;
      _int   [3] = 0;
      _int   [4] = 0;
      _float [0] = 0.0f;
      _float [1] = 0.0f;
      _float [2] = 0.0f;
      _float [3] = 0.0f;
      _float [4] = 0.0f;
      _float [5] = 0.0f;
      _double[0] = 0.0;
      _double[1] = 0.0;
      _double[2] = 0.0;
      _double[3] = 0.0;
      _double[4] = 0.0;
      _double[5] = 0.0;
      _double[6] = 0.0;
      _string[0][0] = '\0';
      _string[1][0] = '\0';
      _string[2][0] = '\0';
      _string[3][0] = '\0';
      _string[4][0] = '\0';
      _string[5][0] = '\0';
      _string[6][0] = '\0';
      _string[7][0] = '\0';
   }

   bool operator == ( const ArrayOfScalar & right ) const {
      return( _int[0] == right._int[0] )
         && ( _int[1] == right._int[1] )
         && ( _int[2] == right._int[2] )
         && ( _int[3] == right._int[3] );
   }

   typedef char string_t[41];

   bool     _bool  [2];
   byte     _byte  [3];
   short    _short [4];
   int      _int   [5];
   float    _float [6];
   double   _double[7];
   string_t _string[8];
};

struct ArrayOfObject : public json::IJSonData {

   virtual const json::CoDec & getCoDec() const;

   void reset() {
      _array[0].reset();
      _array[1].reset();
   }

   bool operator == ( const ArrayOfObject & right ) const {
      return( _array[0] == right._array[0] )
         && ( _array[1] == right._array[1] );
   }

   AllScalar _array[2];
};

struct Composite : public json::IJSonData {

   virtual const json::CoDec & getCoDec() const;

   void reset() {
      _arrayOfScalar.reset();
      _allScalar    .reset();
      _arrayOfObject.reset();
   }

   bool operator == ( const Composite & right ) const {
      return( _arrayOfScalar == right._arrayOfScalar )
         && ( _allScalar     == right._allScalar     )
         && ( _arrayOfObject == right._arrayOfObject );
   }

   ArrayOfScalar _arrayOfScalar;
   AllScalar     _allScalar;
   ArrayOfObject _arrayOfObject;
};

struct AllScalarCoDec : public json::CoDec {

   static AllScalarCoDec codec;

   AllScalarCoDec() :
      json::CoDec(
         new json::Boolean( "bool"  , &AllScalar::_bool,
         new json::Byte   ( "byte"  , &AllScalar::_byte,
         new json::Short  ( "short" , &AllScalar::_short,
         new json::Int    ( "int"   , &AllScalar::_int,
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
         new json::BooleanArray( "bool"  , &ArrayOfScalar::_bool,
         new json::ByteArray   ( "byte"  , &ArrayOfScalar::_byte,
         new json::ShortArray  ( "short" , &ArrayOfScalar::_short,
         new json::IntArray    ( "int"   , &ArrayOfScalar::_int,
         new json::FloatArray  ( "float" , &ArrayOfScalar::_float,
         new json::DoubleArray ( "double", &ArrayOfScalar::_double,
         new json::StringArray ( "string", &ArrayOfScalar::_string ))))))))
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

struct DataCadran : public json::IJSonData {

   virtual const json::CoDec & getCoDec() const;

   char niveauPluieText[500];
   int  niveauPluie;
   char color[7];      // "rrggbb"
};

struct DataCadranCoDec : public json::CoDec {

   static DataCadranCoDec codec;

   DataCadranCoDec() :
      json::CoDec(
         new json::String ( "niveauPluieText", &DataCadran::niveauPluieText,
         new json::Int( "niveauPluie"    , &DataCadran::niveauPluie,
         new json::String ( "color"          , &DataCadran::color ))))
   {}

   void clear( void ) {
      delete _attributes;
      _attributes = 0;
   }
};

DataCadranCoDec DataCadranCoDec::codec;

const json::CoDec & DataCadran::getCoDec() const {
   return DataCadranCoDec::codec;
}

struct PluieDansLHeure : public json::IJSonData {

   virtual const json::CoDec & getCoDec() const;

   char       idLieu[7];               // "332730",
   char       echeance[13];            // "201809122000",
   char       lastUpdate[6];           // "19h50",
   bool       isAvailable;             // true,
   bool       hasData;                 // true
   char       niveauPluieText[80][12]; // [ "De20h00 à 21h00 : Pas de précipitations" ],
   DataCadran dataCadran[12];
};

struct PluieDansLHeureCoDec : public json::CoDec {

   static PluieDansLHeureCoDec codec;

   PluieDansLHeureCoDec() :
      json::CoDec(
         new json::String     ( "idLieu"         , &PluieDansLHeure::idLieu,
         new json::String     ( "echeance"       , &PluieDansLHeure::echeance,
         new json::String     ( "lastUpdate"     , &PluieDansLHeure::lastUpdate,
         new json::Boolean    ( "isAvailable"    , &PluieDansLHeure::isAvailable,
         new json::Boolean    ( "hasData"        , &PluieDansLHeure::hasData,
         new json::StringArray( "niveauPluieText", &PluieDansLHeure::niveauPluieText )))))))
   {}

   void clear( void ) {
      delete _attributes;
      _attributes = 0;
   }
};

PluieDansLHeureCoDec PluieDansLHeureCoDec::codec;

const json::CoDec & PluieDansLHeure::getCoDec() const {
   return PluieDansLHeureCoDec::codec;
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
      Composite decoded;
      decoded.reset();
      json::Status status = d.decode( decoded );
      if( status == json::SUCCESS ) {
         ::fprintf( stderr, "DECODE: ok\n" );
      }
      else {
         if( object == decoded ) {
            ::fprintf( stderr, "DECODE: ok\n" );
         }
         else {
            ::fprintf( stderr, "\33[31mDECODE: BAD : object not affected\33[0m\n" );
         }
      }
   }
   else {
      ::fprintf( stderr, "\33[31mDECODE: BAD : encode error\33[0m\n" );
   }
}

int main( void ) {
   encode( json::SUCCESS        , 1024 );
   encode( json::SUCCESS        ,  845 );
   encode( json::BUFFER_OVERFLOW,  844 );
   decode();
   AllScalarCoDec    ::codec.clear();
   ArrayOfScalarCoDec::codec.clear();
   CompositeCoDec    ::codec.clear();
   return 0;
}
