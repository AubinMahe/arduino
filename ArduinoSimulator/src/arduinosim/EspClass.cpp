#include "ESP8266WiFi.h"

void EspClass::deepSleep( uint64_t time_us, RFMode /* mode = RF_DEFAULT */ ) {
   ::delayMicroseconds( time_us );
}

EspClass ESP;
