#pragma once

#include "Status.h"

#include <initializer_list>

namespace json {

   class Decoder;
   class Encoder;
   class IJSonData;

   class CoDec;
   struct IJSonData {

      virtual ~ IJSonData( void ) {}

      virtual const CoDec & getCoDec() const = 0;
   };

   class AttributeBase {
   public:

      AttributeBase( const char * name, AttributeBase * next );

      virtual ~ AttributeBase( void );

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const = 0;

      virtual Status set( IJSonData & o, Decoder & decoder ) const = 0;

   public:

      bool is( const char * name ) const;

      AttributeBase * next();

   protected:

      const char *    _name;
      AttributeBase * _next;

   private:
      AttributeBase( const AttributeBase & );
      AttributeBase & operator = ( const AttributeBase & );
   };

   class CoDec {
   public:

      CoDec( const char * classname, AttributeBase * attributes ) :
         _classname( classname ),
         _attributes( attributes )
      {}

      virtual ~ CoDec( void ) {}

   public:

      const char * getClassname() const { return _classname; }

      Status encode( const IJSonData & data, Encoder & encoder ) const;

      Status decode( const char * name, IJSonData & data, Decoder & decoder ) const;

   protected:

      const char *    _classname;
      AttributeBase * _attributes;

   private:
      CoDec( const CoDec & );
      CoDec & operator = ( const CoDec & );
   };
}
