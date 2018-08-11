#pragma once

namespace ncurses {

   template<class T, class V>
   class IChangeListener {
   public:

      IChangeListener() {}
      virtual ~ IChangeListener() {}

   public:

      virtual void hasChanged( T & what, V before, V after ) = 0;

   private:
      IChangeListener( const IChangeListener & );
      IChangeListener & operator = ( const IChangeListener & );
   };
}
