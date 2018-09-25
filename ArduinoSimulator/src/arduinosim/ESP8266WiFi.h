#pragma once

#include <Arduino.h>

#undef min
#undef max
#include <string>

struct sockaddr_in;
class WiFiClient {
public:

   WiFiClient();

   WiFiClient( int socket );

   size_t write( const uint8_t * value, size_t count );
   inline size_t write( const char * value, size_t count ) {
      return write((const uint8_t *)value, count );
   }

   size_t print( const char * value );

   size_t println( unsigned int value, int base = DEC );
   size_t println( const char * value );
   size_t println( void );

   operator bool();

   void setTimeout( unsigned long timeout );
   int available();
   uint8_t connected();
   size_t readBytes( char * buffer, size_t length );
   void flush();
   void stop();

private:

   int           _socket;
   unsigned long _timeout;
};

typedef enum {
    WL_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
    WL_IDLE_STATUS      = 0,
    WL_NO_SSID_AVAIL    = 1,
    WL_SCAN_COMPLETED   = 2,
    WL_CONNECTED        = 3,
    WL_CONNECT_FAILED   = 4,
    WL_CONNECTION_LOST  = 5,
    WL_DISCONNECTED     = 6
} wl_status_t;

class IPAddress : public Printable {
public:

   IPAddress();

   virtual size_t printTo( Print& p ) const;

   std::string toString( void ) const;

private:

   union {
      uint8_t  bytes[4];  // IPv4 address
      uint32_t dword;
   } _address;
};

class ESP8266WiFiClass {
public:

   ESP8266WiFiClass();

   wl_status_t begin(
      const char *    ssid,
      const char *    passphrase = NULL,
      int32_t         channel    = 0,
      const uint8_t * bssid      = NULL,
      bool            connect    = true );

   wl_status_t status() const {
      return _status;
   }

   IPAddress localIP() const {
      return _addr;
   }

private:

   wl_status_t _status;
   IPAddress   _addr;
};

extern ESP8266WiFiClass WiFi;
struct MHD_Daemon;

class WiFiServer {
public:

   WiFiServer( uint16_t port );

public:

   void begin();
   WiFiClient available( uint8_t * status = NULL );

private:

   void check( bool isOk, const char * file, int line, const char * call ) const;

private:

   uint16_t _port;
   int      _socket;
};

enum RFMode {
    RF_DEFAULT = 0, // RF_CAL or not after deep-sleep wake up, depends on init data byte 108.
    RF_CAL = 1,      // RF_CAL after deep-sleep wake up, there will be large current.
    RF_NO_CAL = 2,   // no RF_CAL after deep-sleep wake up, there will only be small current.
    RF_DISABLED = 4 // disable RF after deep-sleep wake up, just like modem sleep, there will be the smallest current.
};

class EspClass {
public:
   void deepSleep(uint64_t time_us, RFMode mode = RF_DEFAULT);
};
extern EspClass ESP;
