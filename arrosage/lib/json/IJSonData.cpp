#include "IJSonData.h"
#include "Encoder.h"
#include "Decoder.h"

#include <string.h>
#include <typeinfo>

using namespace json;

AttributeBase::AttributeBase( const char * name, AttributeBase * next ) :
   _name( name ),
   _next( next )
{}

AttributeBase::~ AttributeBase( void ) {
   if( _next ) {
      delete _next;
   }
}

bool AttributeBase::is( const char * name ) const {
   return 0 == ::strcmp( _name,  name );
}

AttributeBase * AttributeBase::next() {
   return _next;
}

CoDec:: ~ CoDec( void ) {
}

Status CoDec::encode( const IJSonData & data, Encoder & encoder ) const {
   Status retVal = encoder.openObject();
   for( auto attr = _attributes; retVal == SUCCESS && attr; attr = attr->next()) {
      if( attr != _attributes ) {
         retVal = encoder.separator();
      }
      if( retVal == SUCCESS ) {
         retVal = attr->put( data, encoder );
      }
   }
   if( retVal == SUCCESS ) {
      retVal = encoder.closeObject();
   }
   return retVal;
}

Status CoDec::decode( const char * name, IJSonData & data, Decoder & decoder ) const {
   for( auto attr = _attributes; attr; attr = attr->next()) {
      if( attr->is( name )) {
         return attr->set( data, decoder );
      }
   }
   snprintf( Decoder::_errMsg, sizeof( Decoder::_errMsg ),
      "Attribute '%s' not found in class '%s'!", name, typeid(*this).name());
   return ATTRIBUTE_NOT_FOUND;
}
