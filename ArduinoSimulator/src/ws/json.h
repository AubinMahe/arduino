#pragma once

#include <cmath>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace hpms {

   struct NamedValueBase {

      NamedValueBase( const std::string & name ) :
         _name( name )
      {}

      virtual ~ NamedValueBase( void ) {}

      virtual std::ostream & operator >> ( std::ostream & stream ) const = 0;

      std::string _name;

      friend std::ostream & operator << ( std::ostream & stream, const NamedValueBase & pair ) {
         return pair >> stream;
      }
   };

   template<class T>
   struct NamedValue : public NamedValueBase {

      NamedValue( const std::string & name, const T & value ) :
         NamedValueBase( name ),
         _value( value )
      {}

      virtual std::ostream & operator >> ( std::ostream & stream ) const {
         return stream << '"' << _name << "\":" << _value;
      }

      T _value;
   };

   template<>
   inline std::ostream & NamedValue<bool>::operator >> ( std::ostream & stream ) const {
      return stream << '"' << _name << "\":" << ( _value ? "true" : "false" );
   }

   template<>
   inline std::ostream & NamedValue<std::string>::operator >> ( std::ostream & stream ) const {
      return stream << '"' << _name << "\":\"" << _value << '"';
   }

   typedef std::shared_ptr<NamedValueBase> sharedPair;

   class JSonParser {
   public:

      JSonParser( const char * begin, unsigned len ) :
         _begin( begin ),
         _end( begin + len ),
         _work( _begin )
      {}

      int skip_spaces() {
         while(( _work < _end )&&( *_work == ' ' )) {
            ++_work;
         }
         return ( _work < _end ) ? *_work : -1;
      }

      std::string string() {
         const char * begin = _work;
         while(( _work < _end )&&(( *_work != '\'' )&&( *_work != '"' ))) {
            ++_work;
         }
         return std::string( begin, _work++ );
      }

      std::string value() {
         const char * begin = _work;
         while(( _work < _end )&&(( *_work != ',' )&&( *_work != '}' )&&( *_work != ' ' ))) {
            ++_work;
         }
         return std::string( begin, _work );
      }

      int parse( std::vector< sharedPair> & pairs ) {
         int c = skip_spaces();
         if( c == '{' ) {
            ++_work;
            while(( _work < _end )&&( c != '}' )) {
               c = skip_spaces();
               if(( c == '\'' )||( c == '"' )) {
                  ++_work;
                  std::string name( string());
                  int c = skip_spaces();
                  if( c == ':' ) {
                     ++_work;
                     c = skip_spaces();
                     std::string v;
                     if(( c == '\'' )||( c == '"' )) {
                        ++_work;
                        v = string();
                        ++_work;
                        v.erase( 0 );
                        v.erase( v.length() - 1 );
                        pairs.push_back( sharedPair( new NamedValue<std::string>( name, v )));
                     }
                     else {
                        v = value();
                        if( v == "true" || v == "false" ) {
                           bool b = ( v == "true" );
                           pairs.push_back( sharedPair( new NamedValue<bool>( name, b )));
                        }
                        else {
                           char * err = 0;
                           long   l   = ::strtol( v.c_str(), &err, 10 );
                           if( *err ) {
                              double d = ::strtod( v.c_str(), &err );
                              if( *err ) {

                              }
                              d = ( *err ? std::nan("") : d );
                              pairs.push_back( sharedPair( new NamedValue<double>( name, d )));
                           }
                           else {
                              pairs.push_back( sharedPair( new NamedValue<int>( name, l )));
                           }
                        }
                        ++_work;
                     }
                  }
               }
               else {
                  return -1;
               }
            }
         }
         return c;
      }

   private:

      const char * _begin;
      const char * _end;
      const char * _work;
   };

   struct pairBuilder {

      pairBuilder( const std::string & name, bool value ) :
         ptr( create( name, value ))
      {}

      pairBuilder( const std::string & name, int value ) :
         ptr( create( name, value ))
      {}

      pairBuilder( const std::string & name, const std::string & value ) :
         ptr( create( name, value ))
      {}

      pairBuilder( const std::string & name, std::initializer_list<pairBuilder> values ) :
         ptr( create( name, values ))
      {}

      sharedPair ptr;

   private:

      sharedPair create( const std::string & name, bool value ) {
          return sharedPair( new NamedValue<bool>( name, value ));
      }

      sharedPair create( const std::string & name, int value ) {
          return sharedPair( new NamedValue<int>( name, value ));
      }

      sharedPair create( const std::string & name, const std::string & value ) {
          return sharedPair( new NamedValue<std::string>( name, value ));
      }

      inline sharedPair create( const std::string & name, std::initializer_list<pairBuilder> values );
   };

   class JSonObject {
   public:

      JSonObject( const char * s, int length ) {
         JSonParser parser( s, length );
         parser.parse( _pairs );
      }

      JSonObject( std::initializer_list<pairBuilder> values ) {
         for( auto & t : values ) {
            add( t.ptr );
         }
      }

   public:

      bool getBoolean( const std::string & name ) const {
         return dynamic_cast<const NamedValue<bool> &>( getNVB( name))._value;
      }

      template<class T = int>
      T getInt( const std::string & name ) const {
         return (T)dynamic_cast<const NamedValue<int> &>( getNVB( name))._value;
      }

      double getDouble( const std::string & name ) const {
         return dynamic_cast<const NamedValue<double> &>( getNVB( name))._value;
      }

      const std::string & getString( const std::string & name ) const {
         return dynamic_cast<const NamedValue<std::string> &>( getNVB( name))._value;
      }

      template<class T>
      T getEnum( const std::string & name ) const {
         return (T)getInt( name );
      }

      std::string serialize() const {
         std::stringstream ss;
         ss << *this;
         return ss.str();
      }

      std::ostream & operator >> ( std::ostream & stream ) const {
         stream << '{';
         for( unsigned i = 0, count = _pairs.size(); i < count; ++i ) {
            const sharedPair & item = _pairs[i];
            const NamedValueBase & ptr = *item;
            stream << ptr;
            if( i < count - 1 ) {
               stream << ',';
            }
         }
         return stream << '}';
      }

   private:

      template<class T>
      void add( const std::string & name, const T & value ) {
         _pairs.push_back( sharedPair( new NamedValue<T>( name, value )));
      }

      void add( sharedPair pair ) {
         _pairs.push_back( pair );
      }

      const NamedValueBase & getNVB( const std::string & name ) const {
         for( auto i = _pairs.begin(); i != _pairs.end(); ++i ) {
            const sharedPair & item = *i;
            if( item->_name == name ) {
               return *item;
            }
         }
         throw std::invalid_argument( name );
      }

   private:

      std::vector<sharedPair> _pairs;

      friend std::ostream & operator << ( std::ostream & stream, const JSonObject & t ) {
         return t >> stream;
      }
   };

   inline sharedPair pairBuilder::create(
      const std::string &                name,
      std::initializer_list<pairBuilder> values )
   {
      return sharedPair( new NamedValue<JSonObject>( name, JSonObject( values )));
   }
}
