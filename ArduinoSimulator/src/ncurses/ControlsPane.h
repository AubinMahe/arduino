#pragma once

#include "Control.h"
#include "Checkbox.h"
#include "Slider.h"
#include "Timeout.h"

#define CHECKBOX_COUNT 14
#define SLIDER_COUNT    6

namespace ncurses {

   class Controls : public Window {
   public:

      Controls() :
         Window( 0, 0, 40, 22 ),
         _hasFocus( false ),
         _focused( 0 )
      {
         char label[40];
         for( auto i = 0U; i < CHECKBOX_COUNT; ++i ) {
            ::sprintf( label, "Digital n°%2d ---", i );
            _controls[i] = new Checkbox( *this, 2, i+1, label );
         }
         for( auto i = 0U; i < SLIDER_COUNT; ++i ) {
            ::sprintf( label, "Analog  n°%2d ---", i );
            auto ndx = CHECKBOX_COUNT + i;
            _controls[ndx] = new Slider( *this, 2, ndx + 1, 17, label, 0, 1023 );
         }
         _controls[_focused] -> setFocus( true );
      }

      ~ Controls() {
         for( auto i = 0; i < CHECKBOX_COUNT + SLIDER_COUNT; ++i ) {
            delete _controls[i];
            _controls[i] = 0;
         }
      }

   public:

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
         _hasFocus = focus;
         _controls[_focused] -> setFocus( focus );
      }

      bool hasFocus() const {
         return _hasFocus;
      }

      void render() {
         for( auto i = 0U; i < SLIDER_COUNT; ++i ) {
            _controls[CHECKBOX_COUNT + i] -> render();
         }
      }

      bool keyPressed( int c ) {
         if( _controls[_focused] -> keyPressed( c )) {
            switch( c ) {
            case KEY_HOME: focusFirst   (); break;
            case KEY_UP  : focusPrevious(); break;
            case KEY_DOWN: focusNext    (); break;
            case KEY_END : focusLast    (); break;
            default: return true;
            }
         }
         _controls[_focused] -> setFocus( true );
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

      void pinMode( uint8_t pin, uint8_t inOrOut ) const {
         if( pin < CHECKBOX_COUNT ) {
            Checkbox * checkbox = (Checkbox *)_controls[pin];
            checkbox->setReadOnly( inOrOut == OUTPUT );
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
         _controls[pin] = new Timeout( *this, 2, pin + 1, control->getLabel(), 179 );
         delete control;
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
         if( _focused ) {
            _controls[_focused] -> setFocus( false );
            _focused = 0;
         }
         _controls[_focused] -> setFocus( true );
      }

      void focusPrevious() {
         _controls[_focused] -> setFocus( false );
         if( --_focused < 0 ) {
            _focused = CHECKBOX_COUNT + SLIDER_COUNT - 1;
         }
         _controls[_focused] -> setFocus( true );
      }

      void focusNext() {
         _controls[_focused] -> setFocus( false );
         if( ++_focused >= CHECKBOX_COUNT + SLIDER_COUNT ) {
            _focused = 0;
         }
         _controls[_focused] -> setFocus( true );
      }

      void focusLast() {
         _controls[_focused] -> setFocus( false );
         _focused = CHECKBOX_COUNT + SLIDER_COUNT - 1;
         _controls[_focused] -> setFocus( true );
      }

   private:

      bool      _hasFocus;
      Control * _controls[CHECKBOX_COUNT + SLIDER_COUNT];
      int       _focused;
   };
}
