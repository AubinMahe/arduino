#ifdef TESTS_UNITAIRES
#include "Encoder.h"
#include "Decoder.h"

#include <string.h>
#include <errno.h>

#include <fstream>

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

struct Instant : public json::IJSonData {

   virtual const json::CoDec & getCoDec() const;

   unsigned char heure;  // 0..23
   unsigned char minute; // 0..59
};
struct InstantCodec : public json::CoDec {

   static const InstantCodec codec;

   InstantCodec() :
      json::CoDec(
         new json::Byte( "heure" , &Instant::heure,
         new json::Byte( "minute", &Instant::minute )))
   {}
};

const InstantCodec InstantCodec::codec;

const json::CoDec & Instant::getCoDec() const {
   return InstantCodec::codec;
}

struct Activite : public json::IJSonData {

   virtual const json::CoDec & getCoDec() const;

   Instant ouverture;
   uint8_t duree;
};
struct ActiviteCodec : public json::CoDec {

   static ActiviteCodec codec;

   ActiviteCodec() :
      json::CoDec(
         new json::Object( "ouverture", &Activite::ouverture,
         new json::Byte  ( "duree"    , &Activite::duree )))
   {}
};

ActiviteCodec ActiviteCodec::codec;

const json::CoDec & Activite::getCoDec() const {
   return ActiviteCodec::codec;
}

struct Vanne : public json::IJSonData {

   const json::CoDec & getCoDec( void ) const;

   uint8_t  pin;
   bool     ouverte;
   Activite matin;
   Activite soir;
};
struct VanneCoDec : public json::CoDec {

   static VanneCoDec codec;

   VanneCoDec() :
      json::CoDec(
         new json::Byte   ( "pin"    , &Vanne::pin,
         new json::Boolean( "ouverte", &Vanne::ouverte,
         new json::Object ( "matin"  , &Vanne::matin,
         new json::Object ( "soir"   , &Vanne::soir )))))
   {}
};

VanneCoDec VanneCoDec::codec;

const json::CoDec & Vanne::getCoDec( void ) const {
   return VanneCoDec::codec;
}

struct Arrosage : public json::IJSonData {

   const json::CoDec & getCoDec( void ) const;

   bool  est_en_marche;
   Vanne vannes[4];
};
struct ArrosageCoDec : public json::CoDec {

   static ArrosageCoDec codec;

   ArrosageCoDec() :
      json::CoDec(
         new json::Boolean    ( "est_en_marche", &Arrosage::est_en_marche,
         new json::ObjectArray( "vannes"       , &Arrosage::vannes )))
   {}
};

ArrosageCoDec ArrosageCoDec::codec;

const json::CoDec & Arrosage::getCoDec( void ) const {
   return ArrosageCoDec::codec;
}

struct DemarrerArreter : public json::IJSonData {

   DemarrerArreter() :
      demarrer( false )
   {}

   virtual const json::CoDec & getCoDec() const;

   bool demarrer;
};

struct DemarrerArreterCoDec : public json::CoDec {

   static const DemarrerArreterCoDec codec;

   DemarrerArreterCoDec() :
      json::CoDec(
         new json::Boolean( "demarrer", &DemarrerArreter::demarrer ))
   {}
};

const DemarrerArreterCoDec DemarrerArreterCoDec::codec;

const json::CoDec & DemarrerArreter::getCoDec() const {
   return DemarrerArreterCoDec::codec;
}

struct CommanderUneVanne : public json::IJSonData {

   CommanderUneVanne() :
      pin( 0 ),
      ouvrir( false )
   {}

   virtual const json::CoDec & getCoDec() const;

   uint8_t pin;
   bool    ouvrir;
};

struct CommanderUneVanneCoDec : public json::CoDec {

   static const CommanderUneVanneCoDec codec;

   CommanderUneVanneCoDec() :
      json::CoDec(
         new json::Byte   ( "pin"   , &CommanderUneVanne::pin,
         new json::Boolean( "ouvrir", &CommanderUneVanne::ouvrir )))
   {}
};

const CommanderUneVanneCoDec CommanderUneVanneCoDec::codec;

const json::CoDec & CommanderUneVanne::getCoDec() const {
   return CommanderUneVanneCoDec::codec;
}

struct CommanderLesVannes : public json::IJSonData {

   virtual const json::CoDec & getCoDec() const;

   CommanderUneVanne les_vannes[100];
};

struct CommanderLesVannesCoDec : public json::CoDec {

   static const CommanderLesVannesCoDec codec;

   CommanderLesVannesCoDec() :
      json::CoDec( new json::ObjectArray( "les_vannes", &CommanderLesVannes::les_vannes ))
   {}
};

const CommanderLesVannesCoDec CommanderLesVannesCoDec::codec;

const json::CoDec & CommanderLesVannes::getCoDec() const {
   return CommanderLesVannesCoDec::codec;
}

template<class A>
struct Commande : public json::IJSonData {

   Commande() {
      ::memset( commande, 0, sizeof( commande ));
   }

   char commande[30];
   A    argument;

   struct CoDec : public json::CoDec {
      CoDec() :
         json::CoDec(
            new json::String( "commande", &Commande::commande,
            new json::Object( "argument", &Commande::argument )))
      {}
   } codec;

   virtual const json::CoDec & getCoDec() const { return codec; }
};

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

struct SerialStub {

   void print( const char * s ) const {
      fprintf( stderr, "%s", s );
   }

   void println( const char * s ) const {
      fprintf( stderr, "%s\n", s );
   }
};

SerialStub Serial;

static void decode( void ) {
   Composite object;
   char buffer[2000];
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
//      json::Decoder::dump((const char *)&object, sizeof( object ), buffer, sizeof( buffer ));
   }
   else {
      ::fprintf( stderr, "\33[31mDECODE: BAD : encode error\33[0m\n" );
   }
   buffer[0] = 0;
   ::strcat( buffer,
      "{\"commande\":\"Charger une configuration\","
      "\"argument\":{"
      "\"est_en_marche\":false,"
      "\"vannes\":[{"
      "\"pin\":1,"
      "\"ouverte\":false,"
      "\"matin\":{"
      "\"ouverture\":{\"heure\":8,\"minute\":0},"
      "\"duree\":15"
      "},"
      "\"soir\":{"
      "\"ouverture\":{\"heure\":22,\"minute\":0},"
      "\"duree\":15"
      "}"
      "},{"
      "\"pin\":2,"
      "\"ouverte\":false,"
      "\"matin\":{"
      "\"ouverture\":{\"heure\":8,\"minute\":0},"
      "\"duree\":15"
      "},"
      "\"soir\":{"
      "\"ouverture\":{\"heure\":8,\"minute\":0},"
      "\"duree\":15"
      "}"
      "},{"
      "\"pin\":3,"
      "\"ouverte\":false,"
      "\"matin\":{"
      "\"ouverture\":{\"heure\":8,\"minute\":0},"
      "\"duree\":15"
      "},"
      "\"soir\":{"
      "\"ouverture\":{\"heure\":8,\"minute\":0},"
      "\"duree\":15"
      "}"
      "},{"
      "\"pin\":4,"
      "\"ouverte\":false,"
      "\"matin\":{"
      "\"ouverture\":{\"heure\":8,\"minute\":0},"
      "\"duree\":15"
      "},"
      "\"soir\":{"
      "\"ouverture\":{\"heure\":8,\"minute\":0},"
      "\"duree\":15"
      "}"
      "}]}}" );
   json::Decoder decoder( buffer, sizeof( buffer ));
   Commande<DemarrerArreter> commande;
   status = decoder.decode( commande );
   if( commande.commande[0] ) {
      decoder.reset();
      Commande<Arrosage> commander_les_vannes;
      status = decoder.decode( commander_les_vannes );
      if( status == json::SUCCESS ) {
         ::fprintf( stderr, "DECODE: ok\n" );
      }
      else {
         ::fprintf( stderr, "\33[31mDECODE: BAD : decode error\33[0m\n" );
      }
   }
   else {
      ::fprintf( stderr, "\33[31mDECODE: BAD : decode error\33[0m\n" );
   }
}

int decodeHTTP( void ) {
   unsigned char buffer[] = {
      0x50,0x4F,0x53,0x54,0x20,0x2F,0x20,0x48,0x54,0x54,0x50,0x2F,0x31,0x2E,0x31,0x0D,
      0x0A,0x48,0x6F,0x73,0x74,0x3A,0x20,0x31,0x39,0x32,0x2E,0x31,0x36,0x38,0x2E,0x31,
      0x2E,0x32,0x34,0x0D,0x0A,0x55,0x73,0x65,0x72,0x2D,0x41,0x67,0x65,0x6E,0x74,0x3A,
      0x20,0x4D,0x6F,0x7A,0x69,0x6C,0x6C,0x61,0x2F,0x35,0x2E,0x30,0x20,0x28,0x58,0x31,
      0x31,0x3B,0x20,0x55,0x62,0x75,0x6E,0x74,0x75,0x3B,0x20,0x4C,0x69,0x6E,0x75,0x78,
      0x20,0x78,0x38,0x36,0x5F,0x36,0x34,0x3B,0x20,0x72,0x76,0x3A,0x36,0x32,0x2E,0x30,
      0x29,0x20,0x47,0x65,0x63,0x6B,0x6F,0x2F,0x32,0x30,0x31,0x30,0x30,0x31,0x30,0x31,
      0x20,0x46,0x69,0x72,0x65,0x66,0x6F,0x78,0x2F,0x36,0x32,0x2E,0x30,0x0D,0x0A,0x41,
      0x63,0x63,0x65,0x70,0x74,0x3A,0x20,0x2A,0x2F,0x2A,0x0D,0x0A,0x41,0x63,0x63,0x65,
      0x70,0x74,0x2D,0x4C,0x61,0x6E,0x67,0x75,0x61,0x67,0x65,0x3A,0x20,0x66,0x72,0x2D,
      0x46,0x52,0x2C,0x66,0x72,0x3B,0x71,0x3D,0x30,0x2E,0x38,0x2C,0x65,0x6E,0x2D,0x55,
      0x53,0x3B,0x71,0x3D,0x30,0x2E,0x35,0x2C,0x65,0x6E,0x3B,0x71,0x3D,0x30,0x2E,0x33,
      0x0D,0x0A,0x41,0x63,0x63,0x65,0x70,0x74,0x2D,0x45,0x6E,0x63,0x6F,0x64,0x69,0x6E,
      0x67,0x3A,0x20,0x67,0x7A,0x69,0x70,0x2C,0x20,0x64,0x65,0x66,0x6C,0x61,0x74,0x65,
      0x0D,0x0A,0x52,0x65,0x66,0x65,0x72,0x65,0x72,0x3A,0x20,0x68,0x74,0x74,0x70,0x3A,
      0x2F,0x2F,0x31,0x39,0x32,0x2E,0x31,0x36,0x38,0x2E,0x31,0x2E,0x32,0x34,0x2F,0x0D,
      0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E,0x74,0x2D,0x74,0x79,0x70,0x65,0x3A,0x20,0x61,
      0x70,0x70,0x6C,0x69,0x63,0x61,0x74,0x69,0x6F,0x6E,0x2F,0x6A,0x73,0x6F,0x6E,0x0D,
      0x0A,0x43,0x6F,0x6E,0x74,0x65,0x6E,0x74,0x2D,0x4C,0x65,0x6E,0x67,0x74,0x68,0x3A,
      0x20,0x36,0x35,0x0D,0x0A,0x44,0x4E,0x54,0x3A,0x20,0x31,0x0D,0x0A,0x43,0x6F,0x6E,
      0x6E,0x65,0x63,0x74,0x69,0x6F,0x6E,0x3A,0x20,0x6B,0x65,0x65,0x70,0x2D,0x61,0x6C,
      0x69,0x76,0x65,0x0D,0x0A,0x0D,0x0A,0x7B,0x22,0x63,0x6F,0x6D,0x6D,0x61,0x6E,0x64,
      0x65,0x22,0x3A,0x22,0x44,0xC3,0xA9,0x6D,0x61,0x72,0x72,0x65,0x72,0x20,0x6F,0x75,
      0x20,0x61,0x72,0x72,0xC3,0xAA,0x74,0x65,0x72,0x22,0x2C,0x22,0x61,0x72,0x67,0x75,
      0x6D,0x65,0x6E,0x74,0x22,0x3A,0x7B,0x22,0x64,0x65,0x6D,0x61,0x72,0x72,0x65,0x72,
      0x22,0x3A,0x74,0x72,0x75,0x65,0x7D,0x7D,0
   };
   size_t receivedBytesCount = sizeof( buffer ) - 1;
   const char * content = 0;
   for( size_t i = 0; i+4 < receivedBytesCount; ++i ) {
      if(  ( buffer[i+0] == 0x0D )
         &&( buffer[i+1] == 0x0A )
         &&( buffer[i+2] == 0x0D )
         &&( buffer[i+3] == 0x0A ))
      {
         content = (char *)(buffer + i + 4);
         break;
      }
   }
   if( content ) {
      ::fprintf( stderr, "content: -%s-\n", content );
      const char * type = ::strstr((char *)buffer, "Content-type:" );
      if( ! type ) {
         type = ::strstr((char *)buffer, "Content-Type:" );
      }
      if( ! type ) {
         type = ::strstr((char *)buffer, "content-type:" );
      }
      if( ! type ) {
         type = ::strstr((char *)buffer, "CONTENT-TYPE:" );
      }
      if( type ) {
         type += 13;
         while( *type == ' ' ) {
            ++type;
         }
      }
      bool isPost = (((char *)buffer) == ::strstr((char *)buffer, "POST / " ));
      bool isJSon = type && (0 == ::strncmp( type, "application/json", 16 ));
      ::fprintf( stderr, "isPost: %s\n", isPost ? "true" : "false" );
      ::fprintf( stderr, "isJSon: %s\n", isJSon ? "true" : "false" );
      ::fprintf( stderr, "type  : %s\n", type );
   }
   else {
      ::fprintf( stderr, "!!!\n" );
   }
   return 0;
}

int main( void ) {
   return decodeHTTP();
   encode( json::SUCCESS        , 1024 );
   encode( json::SUCCESS        ,  845 );
   encode( json::BUFFER_OVERFLOW,  844 );
   decode();
   AllScalarCoDec    ::codec.clear();
   ArrayOfScalarCoDec::codec.clear();
   CompositeCoDec    ::codec.clear();
   return 0;
}
#endif
