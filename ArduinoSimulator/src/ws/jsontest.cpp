#include "json.h"
#include <stdlib.h>

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
}

void jsontest( void ) {
   hpms::JSonObject msg = {
      { "verb"  , ws::E_ANALOG_REFERENCE },
      { "pin"   , 2 },
      { "hiOrLo", true }
   };
   ws::verb_t    verb   = msg.getEnum<ws::verb_t>( "verb" );
   unsigned char pin    = msg.getInt<unsigned char>( "pin" );
   bool          hiOrLo = msg.getBoolean( "hiOrLo" );
   fprintf( stderr, "jsontest|serialization: >%s<\n", msg.serialize().c_str());
   fprintf( stderr, "jsontest|verb  : %d, %d expected\n", verb  , ws::E_ANALOG_REFERENCE );
   fprintf( stderr, "jsontest|pin   : %d, %d expected\n", pin   , 2 );
   fprintf( stderr, "jsontest|hiOrLo: %s, %s expected\n", hiOrLo ? "true" : "false", "true" );
   ::exit( EXIT_SUCCESS );
}
