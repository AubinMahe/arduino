#pragma once

#include "IJSonData.h"

#include <stddef.h>

#include <set>
#include <initializer_list>

namespace json {

   template<class T>
   class Attribute : public AttributeBase {
   public:

      typedef T ( IJSonData::* member_t );

      Attribute( const char * name, member_t member, AttributeBase * next ) :
         AttributeBase( name, next ),
         _member( member )
      {}

   protected:

      member_t _member;
   };

   class Boolean : public Attribute<bool> {
   public:

      template<class C>
      Boolean( const char * name, bool ( C::* member ), AttributeBase * next = 0 ) :
         Attribute<bool>( name, (Attribute::member_t)member, next )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;
   };

   class Byte : public Attribute<unsigned char> {
   public:

      template<class C>
      Byte( const char * name, unsigned char ( C::* member ), AttributeBase * next = 0 ) :
         Attribute<unsigned char>( name, (Attribute::member_t)member, next )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;
   };

   class Short : public Attribute<short> {
   public:

      template<class C>
      Short( const char * name, short ( C::* member ), AttributeBase * next = 0 ) :
         Attribute<short>( name, (Attribute::member_t)member, next )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;
   };

   class Int : public Attribute<int> {
   public:

      template<class C>
      Int( const char * name, int ( C::* member ), AttributeBase * next = 0 ) :
         Attribute<int>( name, (Attribute::member_t)member, next )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;
   };

   class Float : public Attribute<float> {
   public:

      template<class C>
      Float( const char * name, float ( C::* member ), AttributeBase * next = 0) :
         Attribute<float>( name, (Attribute::member_t)member, next )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;
   };

   class Double : public Attribute<double> {
   public:

      template<class C>
      Double( const char * name, double ( C::* member ), AttributeBase * next = 0 ) :
         Attribute<double>( name, (Attribute::member_t)member, next )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;
   };

   class String : public Attribute<char> {
   public:

      template<class C, size_t S>
      String( const char * name, char ( C::* member )[S], AttributeBase * next = 0 ) :
         Attribute<char>( name, (Attribute::member_t)member, next ),
         _size( S )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;

   private:

      size_t _size;
   };

   class BooleanArray : public Attribute<bool> {
   public:

      template<class C, size_t S>
      BooleanArray( const char * name, bool( C::* member )[S], AttributeBase * next = 0 ) :
         Attribute<bool>( name, (Attribute::member_t)member, next ),
         _size( S )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;

   private:

      const size_t _size;
   };

   class ByteArray : public Attribute<unsigned char> {
   public:

      template<class C, size_t S>
      ByteArray( const char * name, unsigned char( C::* member )[S], AttributeBase * next = 0 ) :
         Attribute<unsigned char>( name, (Attribute::member_t)member, next ),
         _size( S )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;

   private:

      const size_t _size;
   };

   class ShortArray : public Attribute<short> {
   public:

      template<class C, size_t S>
      ShortArray( const char * name, short( C::* member )[S], AttributeBase * next = 0 ) :
         Attribute<short>( name, (Attribute::member_t)member, next ),
         _size( S )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;

   private:

      const size_t _size;
   };

   class IntArray : public Attribute<int> {
   public:

      template<class C, size_t S>
      IntArray( const char * name, int( C::* member )[S], AttributeBase * next = 0 ) :
         Attribute<int>( name, (Attribute::member_t)member, next ),
         _size( S )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;

   private:

      const size_t _size;
   };

   class FloatArray : public Attribute<float> {
   public:

      template<class C, size_t S>
      FloatArray( const char * name, float( C::* member )[S], AttributeBase * next = 0 ) :
         Attribute<float>( name, (Attribute::member_t)member, next ),
         _size( S )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;

   private:

      const size_t _size;
   };

   class DoubleArray : public Attribute<double> {
   public:

      template<class C, size_t S>
      DoubleArray( const char * name, double( C::* member )[S], AttributeBase * next = 0 ) :
         Attribute<double>( name, (Attribute::member_t)member, next ),
         _size( S )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;

   private:

      const size_t _size;
   };

   class StringArray : public Attribute<const char *> {
   public:

      template<class C, size_t L, size_t S>
      StringArray( const char * name, char( C::* member )[S][L], AttributeBase * next = 0 ) :
         Attribute<const char *>( name, (Attribute::member_t)member, next ),
         _capacity( L ),
         _size( S )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;

   private:

      const size_t _capacity;
      const size_t _size;
   };

   class Object : public Attribute<IJSonData> {
   public:

      template<class A, class C>
      Object( const char * name, A ( C::* member ), AttributeBase * next = 0 ) :
         Attribute<IJSonData>( name, (Attribute::member_t)member, next )
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;
   };

   class ObjectArray : public Attribute<IJSonData> {
   public:

      template<typename A, typename C, size_t S, typename std::enable_if<std::is_base_of<IJSonData, A>::value>::type* = nullptr>
      ObjectArray( const char * name, A( C::* member )[S], AttributeBase * next = 0 ) :
         Attribute<IJSonData>( name, (Attribute::member_t)member, next ),
         _size( S ),
         _itemSize( sizeof( A ))
      {}

   public:

      virtual Status put( const IJSonData & o, Encoder & encoder ) const;

      virtual Status set( IJSonData & o, Decoder & decoder ) const;

   private:

      size_t _size;
      size_t _itemSize;
   };
}
