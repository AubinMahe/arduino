#include "ESP8266WiFi.h"

ESP8266WiFiClass WiFi;

ESP8266WiFiClass::ESP8266WiFiClass() :
   _status( WL_DISCONNECTED )
{}

wl_status_t ESP8266WiFiClass::begin(
   const char *    /* ssid              */,
   const char *    /* passphrase = NULL */,
   int32_t         /* channel    = 0    */,
   const uint8_t * /* bssid      = NULL */,
   bool            /* connect    = true */ )
{
   return _status = WL_CONNECTED;
}
