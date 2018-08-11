#include <math.h>

#include <Arduino.h>

#include <queue>
#include <thread>
#include <mutex>

#include "UI.h"
#include "ControlsPane.h"
#include "LogPane.h"
#include "StatusPane.h"

namespace ncurses {

   struct UIAttributes {

      Controls    ctrl;
      LogPane     logPane;
      StatusPane  statusPane;
      void    ( * isr_0_func )( void );
      uint8_t     isr_0_mode;
      void    ( * isr_1_func )( void );
      uint8_t     isr_1_mode;
      std::thread *                          thread;
      std::mutex                             jobsLock;
      std::queue<std::function<void(void)> > jobs;
   };
}

using namespace ncurses;

UI::UI() {
   ::initscr();
   ::cbreak();
   ::noecho();
   ::nodelay( stdscr, TRUE );
   ::keypad ( stdscr, TRUE );
   ::refresh();
   ::mousemask( BUTTON1_PRESSED|BUTTON1_RELEASED, 0 );
   a = new UIAttributes();
   a->thread = new std::thread( [ this ] () { run(); });
   for( auto i = 0; i < CHECKBOX_COUNT; ++i ) {
      Checkbox * cb = a->ctrl.getCheckbox( i );
      if( cb ) {
         cb->addChangeListener( this );
      }
   }
}

UI:: ~ UI() {
   delete a->thread;
   delete a;
}

void UI::run( void ) {
   a->ctrl.setFocus( true );
   int c = ::getch();
   a->ctrl.render();
   a->statusPane.setLine1( "Pour démarrer, cliquez puis relâchez le bouton n°2" );
   a->statusPane.setLine2( "Pour faire évoluer l'automate, cliquez puis relâchez le bouton n°2 quand le timeout tourne." );
   while( c != 27 ) {
      MEVENT mouseEvent;
      if( c == KEY_MOUSE && ::getmouse( &mouseEvent ) == OK ) {
         if( mouseEvent.bstate | NCURSES_BUTTON_RELEASED ) {
            Control * control = a->ctrl.getControlAt( mouseEvent.x, mouseEvent.y );
            if( control ) {
               a->logPane.setFocus( false );
               a->ctrl   .setFocus( true  );
               control->  setFocus( true  );
               control-> onMouseEvent( mouseEvent );
            }
            else if( a->logPane.onMouseEvent( mouseEvent )) {
               a->logPane.setFocus( false );
            }
            else {
               a->ctrl   .setFocus( false );
               a->logPane.setFocus( true  );
            }
         }
      }
      else if( a->ctrl.hasFocus()) {
         a->ctrl.keyPressed( c );
      }
      else if( a->logPane.hasFocus()) {
         a->logPane.keyPressed( c );
      }
      {
         std::lock_guard<std::mutex> lock( a->jobsLock );
         while( ! a->jobs.empty()) {
            std::function<void(void)> job = a->jobs.front();
            job();
            a->jobs.pop();
         }
      }
      c = ::getch();
   }
   delete a;
   ::endwin();
   ::exit( EXIT_SUCCESS );
}

//-- Digital I/O ----------------------------------------------------------

int UI::digitalRead( uint8_t pin ) const {
   return a->ctrl.digitalRead( pin );
}

void UI::digitalWrite( uint8_t pin, uint8_t hiOrLo ) const {
   enqueue( [ this, pin, hiOrLo ] () { a->ctrl.digitalWrite( pin, hiOrLo ); });
}

void UI::pinMode( uint8_t pin, uint8_t inOrOut ) const {
   enqueue( [ this, pin, inOrOut ] () { a->ctrl.pinMode( pin, inOrOut ); });
}

//-- Analog I/O -----------------------------------------------------------

int UI::analogRead( uint8_t pin ) const {
   return a->ctrl.analogRead( pin );
}

void UI::analogReference( uint8_t mode ) const {
   enqueue( [ this, mode ] () { a->ctrl.analogReference( mode ); });
}

void UI::analogWrite( uint8_t pin, int value ) const {
   enqueue( [ this, pin, value ] () { a->ctrl.analogWrite( pin, value ); });
}

//-- Advanced I/O ---------------------------------------------------------

void UI::noTone( uint8_t pin ) const {
   enqueue( [ this, pin ] () { a->ctrl.noTone( pin ); });
}

void UI::tone( uint8_t pin, unsigned int frequency, unsigned long duration ) const {
   enqueue( [ this, pin, frequency, duration ] () { a->ctrl.tone( pin, frequency, duration ); });
}

//-- External Interrupts --------------------------------------------------

void UI::attachInterrupt( uint8_t pin, void (* ISR )(void), uint8_t mode ) const {
   if( pin == 2 ) {
      a->isr_0_func = ISR;
      a->isr_0_mode = mode;
   }
   else if( pin == 3 ) {
      a->isr_1_func = ISR;
      a->isr_1_mode = mode;
   }
}

void UI::detachInterrupt( uint8_t pin ) const {
   if( pin == 2 ) {
      a->isr_0_func = 0;
      a->isr_0_mode = 0;
   }
   else if( pin == 3 ) {
      a->isr_1_func = 0;
      a->isr_1_mode = 0;
   }
}

void UI::hasChanged( Checkbox & what, bool, bool after ) {
   if(  ( &what == a->ctrl.getCheckbox( 2 ))
      &&( a->isr_0_func )
      &&(  (( a->isr_0_mode == RISING  ) &&  after )
         ||(( a->isr_0_mode == FALLING ) && !after )))
   {
      a->isr_0_func();
   }
   else if(   ( &what == a->ctrl.getCheckbox( 3 ))
            &&( a->isr_1_func )
            &&(  (( a->isr_1_mode == RISING  ) &&  after )
               ||(( a->isr_1_mode == FALLING ) && !after )))
   {
      a->isr_1_func();
   }
}

//-- Communication --------------------------------------------------------

void UI::print( const char * s ) const {
   a->logPane.print( s );
}

void UI::println( const char * s ) const {
   a->logPane.println( s );
}

//-- Servo -------------------------------------------------------------------

void UI::enqueue( const std::function<void(void)> & job ) const {
   std::lock_guard<std::mutex> lock( a->jobsLock );
   a->jobs.push( job );
}

void UI::servoAttach( uint8_t pin ) const {
   enqueue( [this,pin] () { a->ctrl.servoAttach( pin ); });
}

void UI::servoWrite( uint8_t pin, int value ) const {
   enqueue( [this,pin,value]() { a->ctrl.servoWrite( pin, value ); });
}

void UI::servoDetach( uint8_t pin ) const {
   enqueue( [this,pin]() { a->ctrl.servoDetach( pin ); });
}
