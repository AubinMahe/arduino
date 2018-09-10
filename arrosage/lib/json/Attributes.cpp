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

Status Short::put( const IJSonData & o, Encoder & encoder ) const {
   return encoder.property( _name, (long)( o.*_member ));
}

Status Short::set( IJSonData & o, Decoder & decoder ) const {
   return decoder.decode( o.*_member );
}

Status Integer::put( const IJSonData & o, Encoder & encoder ) const {
   return encoder.property( _name, (long)( o.*_member ));
}

Status Integer::set( IJSonData & o, Decoder & decoder ) const {
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

Status DoubleArray::put( const IJSonData & o, Encoder & encoder ) const {
   const double * source = &( o.*_member );
   return encoder.property( _name, source, _size );
}

Status DoubleArray::set( IJSonData & o, Decoder & decoder ) const {
   double * target = &( o.*_member );
   return decoder.decode( target, _size );
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
