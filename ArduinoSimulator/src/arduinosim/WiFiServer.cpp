#include "ESP8266WiFi.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

WiFiServer::WiFiServer( uint16_t port ) :
   _port( port ),
   _socket( 0 )
{}

void WiFiServer::check( bool isOk, const char * file, int line, const char * call ) const {
   if( isOk ) {
      return;
   }
   ::fprintf( stderr, "%s:%d:%s fails: %s\n",
      file, line, call, ::strerror( errno ));
   ::exit( EXIT_FAILURE );
}

#define CHECK_ZERO(C) check( 0 == C, __FILE__, __LINE__, #C )
#define CHECK_NOT_ZERO(C) check( 0 != (C), __FILE__, __LINE__, #C )

void WiFiServer::begin() {
   int enable = 1;
   CHECK_NOT_ZERO( _socket = ::socket( PF_INET, SOCK_STREAM, 0 ));
   CHECK_ZERO( ::setsockopt( _socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof( int )));
   CHECK_ZERO( ::fcntl( _socket, F_SETFL, O_NONBLOCK ));
   sockaddr_in addrport;
   addrport.sin_family      = AF_INET;
   addrport.sin_port        = htons( _port );
   addrport.sin_addr.s_addr = htonl( INADDR_ANY );
   CHECK_ZERO( ::bind( _socket, (struct sockaddr *) &addrport, sizeof( addrport )));
   CHECK_ZERO( ::listen( _socket, 1 ));
}

WiFiClient WiFiServer::available( uint8_t * status /* = NULL */ ) {
   if( status ) {
      *status = 0;
   }
   sockaddr_in  clientAddr;
   unsigned int clntLen = sizeof( clientAddr );
   int clientSocket = ::accept( _socket, (sockaddr *)&clientAddr, &clntLen );
   if( clientSocket < 0 ) {
      if(( errno == EWOULDBLOCK )||( errno == EAGAIN )) {
         return WiFiClient();
      }
      check( false, __FILE__, __LINE__, "accept( _socket, (sockaddr *)&client, &clntLen )" );
   }
   return WiFiClient( clientSocket );
}
