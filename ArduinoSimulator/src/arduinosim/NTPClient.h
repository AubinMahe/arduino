#pragma once

#include <Udp.h>

class NTPClient {
public:

   NTPClient( UDP & udp, const char * poolServerName, int timeOffset );

   void begin();

   bool update();
   int getHours();
   int getMinutes();

   void end();
};
