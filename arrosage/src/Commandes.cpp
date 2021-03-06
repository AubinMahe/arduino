#include "Commandes.h"

using namespace hpms;

struct EtatDesVannesCoDec : public json::CoDec {

   static const EtatDesVannesCoDec codec;

   EtatDesVannesCoDec() :
      json::CoDec( "EtatDesVannes",
         new json::BooleanArray( "etat_des_vannes", &EtatDesVannes::etat_des_vannes ))
   {}
};

const EtatDesVannesCoDec EtatDesVannesCoDec::codec;

EtatDesVannes::EtatDesVannes( const Arrosage & arrosage ) {
   for( size_t i = 0; i < Arrosage::NBR_VANNES; ++i ) {
      etat_des_vannes[i] = arrosage.vanne_est_ouverte( i );
   }
}

const json::CoDec & EtatDesVannes::getCoDec() const {
   return EtatDesVannesCoDec::codec;
}

struct ConfigurationCoDec : public json::CoDec {

   static const ConfigurationCoDec codec;

   ConfigurationCoDec() :
      json::CoDec( "Configuration",
         new json::Object( "arrosage", &Configuration::arrosage ))
   {}
};

const ConfigurationCoDec ConfigurationCoDec::codec;

const json::CoDec & Configuration::getCoDec() const {
   return ConfigurationCoDec::codec;
}

struct DemarrerArreterCoDec : public json::CoDec {

   static const DemarrerArreterCoDec codec;

   DemarrerArreterCoDec() :
      json::CoDec( "DemarrerArreter",
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
      json::CoDec( "CommanderUneVanne",
         new json::Int  ( "pin" , &CommanderUneVanne::pin,
         new json::Enum8( "etat", &CommanderUneVanne::etat )))
   {}
};

const CommanderUneVanneCoDec CommanderUneVanneCoDec::codec;

const json::CoDec & CommanderUneVanne::getCoDec() const {
   return CommanderUneVanneCoDec::codec;
}

struct CommanderLesVannesCoDec : public json::CoDec {

   static const CommanderLesVannesCoDec codec;

   CommanderLesVannesCoDec() :
      json::CoDec( "CommanderLesVannes",
         new json::ObjectArray( "les_vannes", &CommanderLesVannes::les_vannes ))
   {}
};

const CommanderLesVannesCoDec CommanderLesVannesCoDec::codec;

const json::CoDec & CommanderLesVannes::getCoDec() const {
   return CommanderLesVannesCoDec::codec;
}
