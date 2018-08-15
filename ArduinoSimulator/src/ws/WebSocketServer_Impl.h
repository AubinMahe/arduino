#pragma once

#include <libwebsockets.h>

#include <string.h>
#include <signal.h>

#include <algorithm>
#include <functional>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "json.h"

#include <Arduino.h>

#define CHECKBOX_COUNT 14
#define SLIDER_COUNT    6

namespace ws {

   enum verb_t {

      E_NONE,

      //-- Digital I/O ----------------------------------------------------------

      E_DIGITAL_READ,
      E_DIGITAL_WRITE,
      E_PIN_MODE,

      //-- Analog I/O -----------------------------------------------------------

      E_ANALOG_READ,
      E_ANALOG_REFERENCE,
      E_ANALOG_WRITE,

      //-- Advanced I/O ---------------------------------------------------------

      E_NO_TONE,
      E_TONE,

      //-- External Interrupts --------------------------------------------------

      //-- Communication --------------------------------------------------------

      E_PRINT,
      E_PRINTLN,

      //-- Servo ----------------------------------------------------------------

      E_SERVO_ATTACH,
      E_SERVO_WRITE,
      E_SERVO_DETACH,

      //-- Exit -----------------------------------------------------------------

      E_EXIT
   };

   /* one of these is created for each client connecting to us */
   struct per_session_data__minimal {

      per_session_data__minimal * pss_list;
      lws *                       wsi;
   };

   /* one of these created for each message */
   struct ArdSimData {

      uint8_t header[LWS_PRE];
      uint8_t payload[1024];
      size_t  len;
   };

   /* one of these is created for each vhost our protocol is used with */
   struct per_vhost_data__minimal {

      lws_context *               context;
      lws_vhost *                 vhost;
      const lws_protocols *       protocol;
      per_session_data__minimal * pss_list; /* linked-list of live pss*/
      ArdSimData                  msg;
   };

   static bool _isRunning = false;

   static void exit_requested( int ) {
      _isRunning = false;
      printf( "\n" );
   }

   struct Message {

      unsigned char header[LWS_PRE];
      unsigned char payload[1024];

      bool write( lws * wsi, const std::string & m ) {
         int len = m.length();
         ::strncpy((char *)payload, m.c_str(), len );
         return ::lws_write( wsi, payload, len, LWS_WRITE_TEXT );
      }
   };

   class WebSocketServer_Impl {
   public:

      WebSocketServer_Impl( int port ) :
         _thread( [ this, port ] () { run( port ); })
      {}

   public:

      void enqueue( const std::function<void(void)> & job ) {
         std::lock_guard<std::mutex> lock( _jobsLock );
         _jobs.push( job );
      }

      //-- Digital I/O -------------------------------------------------------------

      int digitalRead( uint8_t pin ) const {
         return _digital[pin];
      }

      void digitalWrite( uint8_t pin, uint8_t hiOrLo ) {
         hpms::JSonObject msg = {
            { "verb"  , E_DIGITAL_WRITE },
            { "pin"   , (int)pin },
            { "hiOrLo", hiOrLo ? true : false },
         };
         sendToUI( msg );
      }

      void pinMode( uint8_t pin, uint8_t mode ) {
         hpms::JSonObject msg = {
            { "verb", E_PIN_MODE },
            { "pin" , (int)pin },
            { "mode", mode ? true : false },
         };
         sendToUI( msg );
      }

      //-- Analog I/O --------------------------------------------------------

      int analogRead( uint8_t pin ) {
         return _analog[pin];
      }

      void analogReference( uint8_t ) {
         // NOT IMPLEMENTED
      }

      void analogWrite( uint8_t pin, int value ) {
         hpms::JSonObject msg = {
            { "verb" , E_ANALOG_WRITE },
            { "pin"  , (int)pin },
            { "value", value },
         };
         sendToUI( msg );
      }

      //-- External Interrupts --------------------------------------------------

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

      //-- Communication -----------------------------------------------------

      void print( const std::string & line ) {
         hpms::JSonObject msg = {
            { "verb", E_PRINT },
            { "line", line },
         };
         sendToUI( msg );
      }

      void println( const std::string & line ) {
         hpms::JSonObject msg = {
            { "verb", E_PRINTLN },
            { "line", line  },
         };
         sendToUI( msg );
      }

      //-- Servo -------------------------------------------------------------------

      void servoAttach( uint8_t pin ) {
         hpms::JSonObject msg = {
            { "verb", E_SERVO_ATTACH },
            { "pin" , (int)pin },
         };
         sendToUI( msg );
      }

      void servoWrite( uint8_t pin, int value ) {
         hpms::JSonObject msg = {
            { "verb" , E_SERVO_WRITE },
            { "pin"  , (int)pin },
            { "value", value },
         };
         sendToUI( msg );
      }

      void servoDetach( uint8_t pin ) {
         hpms::JSonObject msg = {
            { "verb", E_SERVO_DETACH },
            { "pin" , (int)pin },
         };
         sendToUI( msg );
      }

   private:

      void digitalChanged( int pin, bool after ) {
         if(  ( pin == 2 )
            &&( _isr_0_func )
            &&(  (( _isr_0_mode == RISING  ) &&  after )
               ||(( _isr_0_mode == FALLING ) && !after )))
         {
            _isr_0_func();
         }
         else if(
              ( pin == 3 )
            &&( _isr_1_func )
            &&(  (( _isr_1_mode == RISING  ) &&  after )
               ||(( _isr_1_mode == FALLING ) && !after )))
         {
            _isr_1_func();
         }
      }

      void receive( const char * message, int length ) {
         hpms::JSonObject msg( message, length );
         verb_t verb = msg.getEnum<verb_t>( "verb" );
         uint8_t pin;
         bool hiOrLo;
         switch( verb ) {
         case E_NONE: break;
         case E_DIGITAL_READ:
            pin    = msg.getInt<uint8_t>( "pin" );
            hiOrLo = msg.getBoolean( "hiOrLo" );
            _digital[pin] = hiOrLo;
            digitalChanged( pin, hiOrLo );
            break;
         case E_DIGITAL_WRITE:
         case E_PIN_MODE:
         case E_ANALOG_READ:
         case E_ANALOG_REFERENCE:
         case E_ANALOG_WRITE:
         case E_NO_TONE:
         case E_TONE:
         case E_PRINT:
         case E_PRINTLN:
         case E_SERVO_ATTACH:
         case E_SERVO_WRITE:
         case E_SERVO_DETACH:
         case E_EXIT:
            break;
         }
      }

      void sendToUI( const hpms::JSonObject & json ) {
         _msgQueue.push( json.serialize());
         lws_callback_on_writable( _wsi );
      }

      int send( lws * wsi ) {
         const std::string & m = _msgQueue.front();
         if( ! _message.write( wsi, m )) {
            lwsl_err("WebSocketServer_Impl.send|ERROR\n" );
            return -1;
         }
         _msgQueue.pop();
         if( ! _msgQueue.empty()) {
            lws_callback_on_writable( wsi );
         }
         return 0;
      }

      static int callback_minimal(
         lws *                wsi,
         lws_callback_reasons reason,
         void *               user,
         void *               in,
         size_t               len )
      {
         per_session_data__minimal * pss   = (per_session_data__minimal *)user;
         lws_vhost *                 vhost = ::lws_get_vhost   ( wsi );
         const lws_protocols *       proto = ::lws_get_protocol( wsi );
         lws_context *               ctxt  = ::lws_get_context ( wsi );
         per_vhost_data__minimal *   vhd   = (per_vhost_data__minimal *)::lws_protocol_vh_priv_get( vhost, proto );
         ws::WebSocketServer_Impl *  ui    = (ws::WebSocketServer_Impl *)::lws_context_user( ctxt );
         switch( reason ) {
         case LWS_CALLBACK_PROTOCOL_INIT:
            vhd = (per_vhost_data__minimal *)
               ::lws_protocol_vh_priv_zalloc( vhost, proto, sizeof( per_vhost_data__minimal ));
            vhd->context  = ctxt;
            vhd->protocol = proto;
            vhd->vhost    = vhost;
            break;

         case LWS_CALLBACK_ESTABLISHED:
            ::_lws_log( LLL_NOTICE, "ESTABLISHED\n" );
            /* add ourselves to the list of live pss held in the vhd */
            lws_ll_fwd_insert( pss, pss_list, vhd->pss_list );
            pss->wsi = wsi;
            ui->_wsi = wsi;
            if( ! ui->_msgQueue.empty()) {
               ::lws_callback_on_writable_all_protocol( ui->_context, &_protocols[1] );
            }
            break;

         case LWS_CALLBACK_CLOSED:
            ::_lws_log( LLL_NOTICE, "CLOSED\n" );
            /* remove our closing pss from the list of live pss */
            lws_ll_fwd_remove( per_session_data__minimal, pss_list, pss, vhd->pss_list );
            break;

         case LWS_CALLBACK_SERVER_WRITEABLE: return ui->send( wsi );

         case LWS_CALLBACK_RECEIVE:
            ui->receive((const char *)in, len );
            break;

         default:
            break;
         }
         return 0;
      }

      void run( int port ) {
         ::signal( SIGINT, exit_requested );
         lws_context_creation_info info;
         ::lws_set_log_level( LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE | LLL_DEBUG, NULL );
         ::memset( &info, 0, sizeof( info )); /* otherwise uninitialized garbage */
         info.port      = port;
         info.protocols = _protocols;
         info.user      = this;
         lws_context * context = ::lws_create_context( &info );
         if( context ) {
            int n = 0;
            _isRunning = true;
            while(( n >= 0 )&& _isRunning ) {
               n = ::lws_service( context, 20 );
               if( _wsi ) {
                  std::lock_guard<std::mutex> lock( _jobsLock );
                  while( ! _jobs.empty()) {
                     std::function<void(void)> job = _jobs.front();
                     job();
                     _jobs.pop();
                  }
               }
            }
            ::lws_context_destroy( context );
         }
         else {
            lwsl_err( "lws_create_context failed\n" );
         }
         exit( EXIT_SUCCESS );
      }

   private:

      static lws_protocols _protocols[];

      std::queue<
         std::function<
            void(void)> >     _jobs;
      std::mutex              _jobsLock;
      std::thread             _thread;
      lws_context *           _context;
      lws *                   _wsi;
      Message                 _message;
      std::queue<std::string> _msgQueue;
      int                     _digital[CHECKBOX_COUNT];
      void    ( *             _isr_0_func )( void );
      uint8_t                 _isr_0_mode;
      void    ( *             _isr_1_func )( void );
      uint8_t                 _isr_1_mode;
      int                     _analog[SLIDER_COUNT];
   };

   lws_protocols WebSocketServer_Impl::_protocols[] = {
      { "http",
         lws_callback_http_dummy,
         0,
         0, 0, NULL, 0 },
      { "ardsim",
         WebSocketServer_Impl::callback_minimal,
         sizeof( per_session_data__minimal ),
         1024, 0, NULL, 0 },
      { NULL, NULL, 0, 0, 0, NULL, 0 } /* terminator */
   };
}
