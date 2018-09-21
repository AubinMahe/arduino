#include "NTPClient.h"
#include <sys/time.h>
#include <time.h>

NTPClient::NTPClient( UDP &, const char *, int ) {
}

void NTPClient::begin() {}

bool NTPClient::update() {
   return true;
}

int NTPClient::getHours() {
   timeval tv;
   if( 0 == ::gettimeofday( &tv, 0 )) {
      time_t now = tv.tv_sec;
      tm *   t   = ::localtime( &now );
      return t->tm_hour;
   }
   return -1;
}

int NTPClient::getMinutes() {
   timeval tv;
   if( 0 == ::gettimeofday( &tv, 0 )) {
      time_t now = tv.tv_sec;
      tm *   t   = ::localtime( &now );
      return t->tm_min;
   }
   return -1;
}

void NTPClient::end() {}
