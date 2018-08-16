#pragma once

#include "Control.h"
#include "Checkbox.h"
#include "Slider.h"
#include "Timeout.h"

#include <sstream>
#include <iomanip>

namespace std {

   template <class T, std::size_t N>
   constexpr std::size_t size( const T ( & )[ N ]) noexcept {
       return N;
   }
}

#define CHECKBOX_COUNT 14
#define SLIDER_COUNT    6

namespace ncurses {

   class Controls : public Window {
   public:

      explicit Controls( int width, int height ) :
         Window( 0, 0, width, height, ACS_ULCORNER, ACS_TTEE, ACS_LTEE, ACS_BTEE ),
         _hasFocus( false ),
         _focused( -1 ),
         _firstFocusable( -1 )
      {
         int y = 1;
         Checkbox * reset = new Checkbox( *this, 2, y, L"Reset" );
         reset -> setMode( INPUT );
         _controls[y-1] = reset;
         ++y;
         for( int i = 0U; i < CHECKBOX_COUNT; ++i, ++y ) {
            std::wstringstream label;
            label << L"Digital n°" << std::setw(2) << y-2 << std::ends;
            _controls[y-1] = new Checkbox( *this, 2, y, label.str());
         }
         for( int i = 0U; i < SLIDER_COUNT; ++i, ++y ) {
            std::wstringstream label;
            label << L"Analog n°A" << i << std::ends;
            _controls[y-1] = new Slider( *this, 2, y, width - 23, label.str(), 0, 1023 );
         }
      }

      ~ Controls() {
         for( Control * & ctrl : _controls ) {
            delete ctrl;
            ctrl = 0;
         }
      }

   public:

      void addResetListener( IChangeListener<Checkbox, bool> * listener ) {
         ((Checkbox *)_controls[0]) -> addChangeListener( listener );
      }

      virtual void render( bool clear ) {
         Window::render( clear );
         for( Control * ctrl : _controls ) {
            ctrl -> render();
         }
         if( _focused > -1 ) {
            _controls[_focused] -> setFocus( _controls[_focused] -> hasFocus());
         }
         ::wrefresh( w());
      }

      Checkbox * getCheckbox( int pin ) {
         if( pin > 0 && pin <= CHECKBOX_COUNT ) {
            return (Checkbox *)_controls[1+pin];
         }
         return 0;
      }

      Slider * getSlider( int pin ) {
         if( pin > CHECKBOX_COUNT && pin < (int)std::size( _controls )) {
            return (Slider *)_controls[1+pin];
         }
         return 0;
      }

      Control * getControlAt( int x, int y ) {
         for( Control * ctrl : _controls ) {
            if( ctrl -> isHitPoint( x, y )) {
               return ctrl;
            }
         }
         return 0;
      }

      void setFocus( bool focus ) {
         if( _focused > -1 ) {
            _controls[_focused] -> setFocus( focus );
         }
         _hasFocus = focus;
         if( focus && _focused < 0 ) {
            _firstFocusable = -1;
            for( int i = 0; i < (int)std::size( _controls ); ++i ) {
               if( _controls[i] -> isFocusable()) {
                  _focused        = i;
                  _firstFocusable = i;
                  break;
               }
            }
            if( _focused > -1 ) {
               _controls[_focused] -> setFocus( focus );
               _hasFocus = true;
            }
            else {
               _controls[_focused] -> setFocus( focus );
            }
         }
         render( false );
      }

      bool hasFocus() const {
         return _hasFocus;
      }

      bool keyPressed( int key ) {
         if( _focused > -1 ) {
            if( _controls[_focused] -> keyPressed( key )) {
               switch( key ) {
               case KEY_HOME: focusFirst   (); break;
               case KEY_UP  : focusPrevious(); break;
               case KEY_DOWN: focusNext    (); break;
               case KEY_END : focusLast    (); break;
               default: return true;
               }
            }
            _controls[_focused] -> setFocus( true );
            ::wrefresh( w());
         }
         return false;
      }

   public:

      //-- Digital I/O -------------------------------------------------------

      bool digitalRead( uint8_t pin ) const {
         if( pin < CHECKBOX_COUNT ) {
            const Checkbox * checkbox = (const Checkbox *)_controls[1+pin];
            return checkbox->isChecked();
         }
         return false;
      }

      void digitalWrite( uint8_t pin, bool value ) {
         if( pin < CHECKBOX_COUNT ) {
            Checkbox * checkbox = (Checkbox *)_controls[1+pin];
            checkbox->setChecked( value );
         }
      }

      void pinMode( uint8_t pin, uint8_t mode, IChangeListener<Checkbox, bool> * listener ) {
         if( pin < CHECKBOX_COUNT ) {
            Checkbox * checkbox = (Checkbox *)_controls[1+pin];
            checkbox->setMode( mode );
            if( mode == INPUT ) {
               checkbox->addChangeListener( listener );
               setFocus( true );
            }
         }
      }

      //-- Analog I/O --------------------------------------------------------

      int analogRead( uint8_t pin ) const {
         if( pin < SLIDER_COUNT ) {
            const Slider * slider = (const Slider *)_controls[1+CHECKBOX_COUNT+pin];
            return slider->getValue();
         }
         return false;
      }

      void analogReference( uint8_t/*mode*/) const {
      }

      void analogWrite( uint8_t pin, int value ) const {
         if( pin < SLIDER_COUNT ) {
            Slider * slider = (Slider *)_controls[1+CHECKBOX_COUNT+pin];
            slider->setValue( value );
         }
      }

      //-- Advanced I/O ------------------------------------------------------

      void noTone( uint8_t/*pin*/) const {
      }

      void tone( uint8_t/*pin*/, unsigned int/*frequency*/, unsigned long/*duration*/) const {
      }

      //-- External Interrupts -----------------------------------------------
      //-- Communication -----------------------------------------------------
      //-- Servo -------------------------------------------------------------

      void servoAttach( uint8_t pin ) {
         if( pin < CHECKBOX_COUNT ) {
            Control * control = _controls[1+pin];
            _controls[1+pin] = new Timeout( *this, 2, pin + 2, control->getLabel(), 0 );
            delete control;
            render( false );
         }
      }

      void servoWrite( uint8_t pin, int value ) const {
         if( pin < CHECKBOX_COUNT ) {
            ((Timeout *)_controls[1+pin])->setRemaining( value );
         }
      }

      void servoDetach( uint8_t pin ) {
         if( pin < CHECKBOX_COUNT ) {
            Control * control = _controls[pin+1];
            _controls[1+pin] = new Checkbox( *this, 2, pin + 2, control->getLabel());
            delete control;
         }
      }

   private:

      void focusFirst() {
         if( _firstFocusable < 0 ) {
            return;
         }
         if( _focused > 0 ) {
            _controls[_focused] -> setFocus( false );
            _focused = 0;
         }
         if( ! _controls[_focused] -> isFocusable()) {
            focusNext();
         }
         else {
            _controls[_focused] -> setFocus( true );
         }
      }

      void focusNext() {
         if( _firstFocusable < 0 ) {
            return;
         }
         if( _focused > -1 ) {
            _controls[_focused] -> setFocus( false );
         }
         if( ++_focused >= (int)std::size( _controls )) {
            _focused = 0;
         }
         if( ! _controls[_focused] -> isFocusable()) {
            focusNext();
         }
         else {
            _controls[_focused] -> setFocus( true );
         }
      }

      void focusPrevious() {
         if( _firstFocusable < 0 ) {
            return;
         }
         if( _focused > -1 ) {
            _controls[_focused] -> setFocus( false );
         }
         if( --_focused < 0 ) {
            _focused = (int)std::size( _controls ) - 1;
         }
         if( ! _controls[_focused] -> isFocusable()) {
            focusPrevious();
         }
         else {
            _controls[_focused] -> setFocus( true );
         }
      }

      void focusLast() {
         if( _firstFocusable < 0 ) {
            return;
         }
         if( _focused > -1 ) {
            _controls[_focused] -> setFocus( false );
         }
         _focused = (int)std::size( _controls ) - 1;
         if( ! _controls[_focused] -> isFocusable()) {
            focusPrevious();
         }
         else {
            _controls[_focused] -> setFocus( true );
         }
      }

   private:

      bool      _hasFocus;
      Control * _controls[ 1 + CHECKBOX_COUNT + SLIDER_COUNT];
      int       _focused;
      int       _firstFocusable;
   };
}
