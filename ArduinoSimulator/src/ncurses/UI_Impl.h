#pragma once

#include <math.h>
#include <locale.h>
#include <string.h>

#include <Arduino.h>

#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#include "UI.h"
#include "IChangeListener.h"
#include "ControlsPane.h"
#include "LogPane.h"
#include "StatusPane.h"
#include "getFileContents.h"

namespace ncurses {

   class UI_Impl : public IChangeListener<Checkbox, bool> {
   public:

      UI_Impl() :
         _isr_0_func( 0 ),
         _isr_0_mode( 0 ),
         _isr_1_func( 0 ),
         _isr_1_mode( 0 ),
         _ctrl( 0 ),
         _logPane( 0 ),
         _statusPane( 0 ),
         _thread( [ this ] () { run(); })
      {}

   public:

      virtual void hasChanged( Checkbox & what, bool, bool after ) {
         if(  ( &what == _ctrl->getCheckbox( 2 ))
            &&( _isr_0_func )
            &&(  (( _isr_0_mode == RISING  ) &&  after )
               ||(( _isr_0_mode == FALLING ) && !after )))
         {
            _isr_0_func();
         }
         else if(   ( &what == _ctrl->getCheckbox( 3 ))
                  &&( _isr_1_func )
                  &&(  (( _isr_1_mode == RISING  ) &&  after )
                     ||(( _isr_1_mode == FALLING ) && !after )))
         {
            _isr_1_func();
         }
      }

      void attachInterrupt( uint8_t pin, void (* ISR )(void), uint8_t mode ) {
         if( pin == 2 ) {
            _isr_0_func = ISR;
            _isr_0_mode = mode;
         }
         else if( pin == 3 ) {
            _isr_1_func = ISR;
            _isr_1_mode = mode;
         }
      }

      void detachInterrupt( uint8_t pin ) {
         if( pin == 2 ) {
            _isr_0_func = 0;
            _isr_0_mode = 0;
         }
         else if( pin == 3 ) {
            _isr_1_func = 0;
            _isr_1_mode = 0;
         }
      }

      Controls & ctrl() {
         return *_ctrl;
      }

      LogPane & log() {
         return *_logPane;
      }

      void enqueue( const std::function<void(void)> & job ) {
         std::lock_guard<std::mutex> lock( _jobsLock );
         _jobs.push( job );
      }

   private:

      void run( void ) {
         ::setlocale( LC_ALL, "" );
         ::initscr();
         ::cbreak();
         ::noecho();
         ::nodelay( stdscr, TRUE );
         ::keypad ( stdscr, TRUE );
         ::mousemask( BUTTON1_PRESSED|BUTTON1_RELEASED, 0 );
         ::refresh();
         _ctrl       = new Controls( 50, 22 );
         _logPane    = new LogPane( _ctrl->getWidth() - 1, 40, _ctrl->getHeight());
         _statusPane =
            new StatusPane( _ctrl->getHeight() - 1, _ctrl->getWidth() + _logPane->getWidth() - 1 );
         auto lines = hpms::getFileContents( "instructions.txt" );
         if( lines.size() > 0 ) {
            _statusPane->setLine1( lines[0].c_str());
            if( lines.size() > 1 ) {
               _statusPane->setLine2( lines[1].c_str());
            }
         }
         _ctrl->setFocus( true );
         MEVENT m;
         int key = ERR;
         while( key != 27 ) {
            if( key == KEY_RESIZE ) {
               ::clear();
               ::refresh();
               if( COLS < 120 && COLS > 86 ) {
                  _logPane   ->setWidth( COLS - _ctrl->getWidth());
                  _statusPane->setWidth( COLS - 1 );
               }
               _ctrl      ->render( true );
               _logPane   ->render( true );
               _statusPane->render( true );
            }
            else if(   ( key == KEY_MOUSE )
                     &&( ::getmouse( &m ) != ERR )
                     &&( m.bstate | NCURSES_BUTTON_RELEASED ))
            {
               Control * control = _ctrl->getControlAt( m.x, m.y );
               if( control ) {
                  _logPane->setFocus( false );
                  _ctrl   ->setFocus( true  );
                  control ->setFocus( true  );
                  control ->onMouseEvent( m );
               }
               else if( _logPane->onMouseEvent( m )) {
                  _logPane->setFocus( false );
               }
               else {
                  _ctrl   ->setFocus( false );
                  _logPane->setFocus( true  );
               }
            }
            else if( key > 0 && _ctrl->hasFocus()) {
               _ctrl->keyPressed( key );
            }
            else if( key > 0 && _logPane->hasFocus()) {
               _logPane->keyPressed( key );
            }
            {
               std::lock_guard<std::mutex> lock( _jobsLock );
               while( ! _jobs.empty()) {
                  std::function<void(void)> job = _jobs.front();
                  job();
                  _jobs.pop();
               }
            }
            if( _ctrl->hasFocus()) {
               _ctrl->setFocus( true );
            }
            else if( _logPane->hasFocus()) {
               _logPane->setFocus( true );
            }
            timespec spec;
            spec.tv_sec  = 0;
            spec.tv_nsec = 20 * 1000 * 1000; // 20 ms
            nanosleep( &spec, 0 );
            key = ::getch();
         }
         ::endwin();
         ::exit( EXIT_SUCCESS );
      }

   private:

      std::queue<
         std::function<
            void(void)> > _jobs;
      std::mutex          _jobsLock;
      void    ( *         _isr_0_func )( void );
      uint8_t             _isr_0_mode;
      void    ( *         _isr_1_func )( void );
      uint8_t             _isr_1_mode;
      Controls *          _ctrl;
      LogPane *           _logPane;
      StatusPane *        _statusPane;
      std::thread         _thread;
   };
}
