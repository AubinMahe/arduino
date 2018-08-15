#pragma once

#include "Control.h"
#include "Checkbox.h"
#include "Slider.h"
#include "Timeout.h"

#include <sstream>
#include <iomanip>

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
         for( int i = 0U; i < CHECKBOX_COUNT; ++i, ++y ) {
            std::wstringstream label;
            label << L"Digital n°" << std::setw(2) << i << std::ends;
            _controls[i] = new Checkbox( *this, 2, y, label.str());
         }
         for( int i = 0U; i < SLIDER_COUNT; ++i, ++y ) {
            std::wstringstream label;
            label << L"Analog  n°" << std::setw(2) << i << std::ends;
            _controls[y-1] = new Slider( *this, 2, y, width - 24, label.str(), 0, 1023 );
         }
      }

      ~ Controls() {
         for( auto i = 0; i < CHECKBOX_COUNT + SLIDER_COUNT; ++i ) {
            delete _controls[i];
            _controls[i] = 0;
         }
      }

   public:

      virtual void render( bool clear ) {
         Window::render( clear );
         for( auto i = 0U; i < CHECKBOX_COUNT+SLIDER_COUNT; ++i ) {
            _controls[i] -> render();
         }
         if( _focused > -1 ) {
            _controls[_focused] -> setFocus( _controls[_focused] -> hasFocus());
         }
         ::wrefresh( w());
      }

      Checkbox * getCheckbox( int index ) {
         if( index < CHECKBOX_COUNT ) {
            return (Checkbox *)_controls[index];
         }
         return 0;
      }

      Slider * getSlider( int index ) {
         if( index >= CHECKBOX_COUNT && index < CHECKBOX_COUNT + SLIDER_COUNT ) {
            return (Slider *)_controls[index];
         }
         return 0;
      }

      Control * getControlAt( int x, int y ) {
         for( auto i = 0; i < CHECKBOX_COUNT + SLIDER_COUNT; ++i ) {
            if( _controls[i] -> isHitPoint( x, y )) {
               return _controls[i];
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
            for( int i = 0; i < CHECKBOX_COUNT + SLIDER_COUNT; ++i ) {
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
               _hasFocus = false;
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
            const Checkbox * checkbox = (const Checkbox *)_controls[pin];
            return checkbox->isChecked();
         }
         return false;
      }

      void digitalWrite( uint8_t pin, bool value ) {
         if( pin < CHECKBOX_COUNT ) {
            Checkbox * checkbox = (Checkbox *)_controls[pin];
            checkbox->setChecked( value );
         }
      }

      void pinMode( uint8_t pin, uint8_t mode, IChangeListener<Checkbox, bool> * listener ) {
         if( pin < CHECKBOX_COUNT ) {
            Checkbox * checkbox = (Checkbox *)_controls[pin];
            checkbox->setMode( mode );
            if( mode == INPUT ) {
               checkbox->addChangeListener( listener );
               setFocus( true );
            }
         }
      }

      //-- Analog I/O --------------------------------------------------------

      int analogRead( uint8_t pin ) const {
         if( pin >= CHECKBOX_COUNT && pin < CHECKBOX_COUNT+SLIDER_COUNT) {
            const Slider * slider = (const Slider *)_controls[pin];
            return slider->getValue();
         }
         return false;
      }

      void analogReference( uint8_t/*mode*/) const {
      }

      void analogWrite( uint8_t pin, int value ) const {
         if( pin >= CHECKBOX_COUNT && pin < CHECKBOX_COUNT+SLIDER_COUNT) {
            Slider * slider = (Slider *)_controls[pin];
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
         Control * control = _controls[pin];
         _controls[pin] = new Timeout( *this, 2, pin + 1, control->getLabel(), 0 );
         delete control;
         render( false );
      }

      void servoWrite( uint8_t pin, int value ) const {
         ((Timeout *)_controls[pin])->setRemaining( value );
      }

      void servoDetach( uint8_t pin ) {
         Control * control = _controls[pin];
         _controls[pin] = new Checkbox( *this, 2, pin + 1, control->getLabel());
         delete control;
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
         if( ++_focused >= CHECKBOX_COUNT + SLIDER_COUNT ) {
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
            _focused = CHECKBOX_COUNT + SLIDER_COUNT - 1;
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
         _focused = CHECKBOX_COUNT + SLIDER_COUNT - 1;
         if( ! _controls[_focused] -> isFocusable()) {
            focusPrevious();
         }
         else {
            _controls[_focused] -> setFocus( true );
         }
      }

   private:

      bool      _hasFocus;
      Control * _controls[CHECKBOX_COUNT + SLIDER_COUNT];
      int       _focused;
      int       _firstFocusable;
   };
}
