#include "Commandes.h"

using namespace hpms;

struct ConfigurationCoDec : public json::CoDec {

   static const ConfigurationCoDec codec;

   ConfigurationCoDec() :
      json::CoDec(
         new json::Object( "arrosage", &Configuration::arrosage,
         new json::Object( "heure"   , &Configuration::heure )))
   {}
};

const ConfigurationCoDec ConfigurationCoDec::codec;

const json::CoDec & Configuration::getCoDec() const {
   return ConfigurationCoDec::codec;
}

struct DemarrerArreterCoDec : public json::CoDec {

   static const DemarrerArreterCoDec codec;

   DemarrerArreterCoDec() :
      json::CoDec(
         new json::Boolean( "demarrer", &DemarrerArreter::demarrer ))
   {}
};

const DemarrerArreterCoDec DemarrerArreterCoDec::codec;

const json::CoDec & DemarrerArreter::getCoDec() const {
   return DemarrerArreterCoDec::codec;
}

struct CommanderUneVanneCoDec : public json::CoDec {

   static const CommanderUneVanneCoDec codec;

   CommanderUneVanneCoDec() :
      json::CoDec(
         new json::Byte   ( "pin"   , &CommanderUneVanne::pin,
         new json::Boolean( "ouvrir", &CommanderUneVanne::ouvrir )))
   {}
};

const CommanderUneVanneCoDec CommanderUneVanneCoDec::codec;

const json::CoDec & CommanderUneVanne::getCoDec() const {
   return CommanderUneVanneCoDec::codec;
}
