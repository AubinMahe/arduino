#include "Attributes.h"
#include "Encoder.h"
#include "Decoder.h"

using namespace json;

Status Boolean::put( const IJSonData & o, Encoder & encoder ) const {
   return encoder.property( _name, o.*_member );
}

Status Boolean::set( IJSonData & o, Decoder & decoder ) const {
   return decoder.decode( o.*_member );
}

Status Byte::put( const IJSonData & o, Encoder & encoder ) const {
   return encoder.property( _name, (long)( o.*_member ));
}

Status Byte::set( IJSonData & o, Decoder & decoder ) const {
   return decoder.decode( o.*_member );
}

Status Enum8::put( const IJSonData & o, Encoder & encoder ) const {
   return encoder.property( _name, (long)( o.*_member ));
}

Status Enum8::set( IJSonData & o, Decoder & decoder ) const {
   return decoder.decode( o.*_member );
}

Status Short::put( const IJSonData & o, Encoder & encoder ) const {
   return encoder.property( _name, (long)( o.*_member ));
}

Status Short::set( IJSonData & o, Decoder & decoder ) const {
   return decoder.decode( o.*_member );
}

Status Int::put( const IJSonData & o, Encoder & encoder ) const {
   return encoder.property( _name, (long)( o.*_member ));
}

Status Int::set( IJSonData & o, Decoder & decoder ) const {
   return decoder.decode( o.*_member );
}

Status Float::put( const IJSonData & o, Encoder & encoder ) const {
   return encoder.property( _name, o.*_member );
}

Status Float::set( IJSonData & o, Decoder & decoder ) const {
   return decoder.decode( o.*_member );
}

Status Double::put( const IJSonData & o, Encoder & encoder ) const {
   return encoder.property( _name, o.*_member );
}

Status Double::set( IJSonData & o, Decoder & decoder ) const {
   return decoder.decode( o.*_member );
}

Status String::put( const IJSonData & o, Encoder & encoder ) const {
   const char * source = &( o.*_member );
   return encoder.property( _name, source );
}

Status String::set( IJSonData & o, Decoder & decoder ) const {
   char * target = &( o.*_member );
   return decoder.decode( _size, target );
}

Status BooleanArray::put( const IJSonData & o, Encoder & encoder ) const {
   const bool * source = &( o.*_member );
   return encoder.property( _name, source, _size );
}

Status BooleanArray::set( IJSonData & o, Decoder & decoder ) const {
   bool * target = &( o.*_member );
   return decoder.decode( target, _size );
}

Status ByteArray::put( const IJSonData & o, Encoder & encoder ) const {
   const unsigned char * source = &( o.*_member );
   return encoder.property( _name, source, _size );
}

Status ByteArray::set( IJSonData & o, Decoder & decoder ) const {
   unsigned char * target = &( o.*_member );
   return decoder.decode( target, _size );
}

Status ShortArray::put( const IJSonData & o, Encoder & encoder ) const {
   const short * source = &( o.*_member );
   return encoder.property( _name, source, _size );
}

Status ShortArray::set( IJSonData & o, Decoder & decoder ) const {
   short * target = &( o.*_member );
   return decoder.decode( target, _size );
}

Status IntArray::put( const IJSonData & o, Encoder & encoder ) const {
   const int * source = &( o.*_member );
   return encoder.property( _name, source, _size );
}

Status IntArray::set( IJSonData & o, Decoder & decoder ) const {
   int * target = &( o.*_member );
   return decoder.decode( target, _size );
}

Status FloatArray::put( const IJSonData & o, Encoder & encoder ) const {
   const float * source = &( o.*_member );
   return encoder.property( _name, source, _size );
}

Status FloatArray::set( IJSonData & o, Decoder & decoder ) const {
   float * target = &( o.*_member );
   return decoder.decode( target, _size );
}

Status DoubleArray::put( const IJSonData & o, Encoder & encoder ) const {
   const double * source = &( o.*_member );
   return encoder.property( _name, source, _size );
}

Status DoubleArray::set( IJSonData & o, Decoder & decoder ) const {
   double * target = &( o.*_member );
   return decoder.decode( target, _size );
}

Status StringArray::put( const IJSonData & o, Encoder & encoder ) const {
   const char * source = (const char *)&( o.*_member );
   return encoder.property( _name, source, _capacity, _size );
}

Status StringArray::set( IJSonData & o, Decoder & decoder ) const {
   char * target = (char *)&( o.*_member );
   return decoder.decode( target, _capacity, _size );
}

Status Object::put( const IJSonData & o, Encoder & encoder ) const {
   return encoder.property( _name, o.*_member );
}

Status Object::set( IJSonData & o, Decoder & decoder ) const {
   return decoder.decode( o.*_member );
}

Status ObjectArray::put( const IJSonData & o, Encoder & encoder ) const {
   const IJSonData * source = &( o.*_member );
   return encoder.property( _name, source, _size, _itemSize );
}

Status ObjectArray::set( IJSonData & o, Decoder & decoder ) const {
   IJSonData * target = &( o.*_member );
   return decoder.decode( target, _size, _itemSize );
}
