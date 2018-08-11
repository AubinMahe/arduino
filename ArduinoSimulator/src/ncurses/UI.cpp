#include <math.h>
#include <string.h>
#include <locale.h>

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

      UIAttributes() :
         ctrl( 50, 22 ),
         logPane( ctrl.getWidth(), ctrl.getHeight()),
         statusPane( ctrl.getHeight(), ctrl.getWidth() + logPane.getWidth()),
         isr_0_func( 0 ),
         isr_0_mode( 0 ),
         isr_1_func( 0 ),
         isr_1_mode( 0 ),
         thread( 0 )
      {
         fprintf( stderr, "ctrl.Width = %d, logPane.Width = %d )\n",
            ctrl.getWidth(), logPane.getWidth());
      }

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
   ::mousemask( BUTTON1_PRESSED|BUTTON1_RELEASED, 0 );
   ::refresh();
   a = new UIAttributes();
   a->thread = new std::thread( [ this ] () { run(); });
}

UI:: ~ UI() {
   delete a->thread;
   delete a;
}

static std::vector<std::string> getFileContents( const char * name ) {
   std::vector<std::string> lines;
   FILE * instructions = fopen( name, "r" );
   if( instructions ) {
      char line[1024];
      while( fgets( line, sizeof( line ), instructions )) {
         lines.push_back( strtok( line, "\n" ));
      }
      fclose( instructions );
   }
   return lines;
}

// ncurses thread.
void UI::run( void ) {
   auto lines = getFileContents( "instructions.txt" );
   if( lines.size() > 0 ) {
      a->statusPane.setLine1( lines[0].c_str());
      if( lines.size() > 1 ) {
         a->statusPane.setLine2( lines[1].c_str());
      }
   }
   a->ctrl.setFocus( true );
   a->ctrl.render();
   int c = ERR;
   while( c != 27 ) {
      MEVENT mouseEvent;
      if( c == KEY_MOUSE && ::getmouse( &mouseEvent ) != ERR ) {
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
      else if( c > 0 && a->ctrl.hasFocus()) {
         a->ctrl.keyPressed( c );
      }
      else if( c > 0 && a->logPane.hasFocus()) {
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
      if( a->ctrl.hasFocus()) {
         a->ctrl.setFocus( true );
      }
      else if( a->logPane.hasFocus()) {
         a->logPane.setFocus( false );
      }
      timespec spec;
      spec.tv_sec  = 0;
      spec.tv_nsec = 20 * 1000 * 1000; // 20 ms
      nanosleep( &spec, 0 );
      c = ::getch();
   }
   delete a;
   ::endwin();
   ::exit( EXIT_SUCCESS );
}

// ncurses job queue management
void UI::enqueue( const std::function<void(void)> & job ) const {
   std::lock_guard<std::mutex> lock( a->jobsLock );
   a->jobs.push( job );
}

//-- Digital I/O ----------------------------------------------------------

int UI::digitalRead( uint8_t pin ) const {
   return a->ctrl.digitalRead( pin );
}

void UI::digitalWrite( uint8_t pin, uint8_t hiOrLo ) const {
   enqueue( [ this, pin, hiOrLo ] () { a->ctrl.digitalWrite( pin, hiOrLo ); });
}

void UI::pinMode( uint8_t pin, uint8_t mode ) const {
   UI * notifiable = const_cast<UI * >( this );
   enqueue( [ this, notifiable, pin, mode ] () { a->ctrl.pinMode( pin, mode, notifiable ); });
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

void UI::print( const char * line ) const {
   enqueue( [ this, line ] () { a->logPane.print( line ); });
}

void UI::println( const char * line ) const {
   enqueue( [ this, line ] () { a->logPane.println( line ); });
}

//-- Servo -------------------------------------------------------------------

void UI::servoAttach( uint8_t pin ) const {
   enqueue( [this,pin] () { a->ctrl.servoAttach( pin ); });
}

void UI::servoWrite( uint8_t pin, int value ) const {
   enqueue( [this,pin,value]() { a->ctrl.servoWrite( pin, value ); });
}

void UI::servoDetach( uint8_t pin ) const {
   enqueue( [this,pin]() { a->ctrl.servoDetach( pin ); });
}
