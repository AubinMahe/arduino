var ardsim = {

   E_NONE            :  0,
   //-- Digital I/O ----------------------------------------------------------
   E_DIGITAL_READ    :  1,
   E_DIGITAL_WRITE   :  2,
   E_PIN_MODE        :  3,
   //-- Analog I/O -----------------------------------------------------------
   E_ANALOG_READ     :  4,
   E_ANALOG_REFERENCE:  5,
   E_ANALOG_WRITE    :  6,
   //-- Advanced I/O ---------------------------------------------------------
   E_NO_TONE         :  7,
   E_TONE            :  8,
   //-- Communication --------------------------------------------------------
   E_PRINT           :  9,
   E_PRINTLN         : 10,
   //-- Servo ----------------------------------------------------------------
   E_SERVO_ATTACH    : 11,
   E_SERVO_WRITE     : 12,
   E_SERVO_DETACH    : 13,
   //-- Exit -----------------------------------------------------------------
   E_EXIT            : 14,

   ws    : null,
   create: function() {
      try {
         ardsim.ws = new WebSocket( "ws://localhost:2416", "ardsim" );
         ardsim.ws.onopen = function() {
            let title = document.getElementById( "ardsim-title" );
            title.textContent = "Arduino simulator : connecté";
         };
         ardsim.ws.onclose = function() {
            let title = document.getElementById( "ardsim-title" );
            title.textContent = "Arduino simulator : déconnecté";
         };
         ardsim.ws.onmessage = function( msg ) {
//            console.log( "%o", msg.data );
            let m = JSON.parse( msg.data );
            switch( m.verb ) {
            //-- Digital I/O -------------------------------------------------
            case ardsim.E_DIGITAL_WRITE: ardsim.digitalWrite( m.pin, m.hiOrLo ); break;
            case ardsim.E_PIN_MODE     : ardsim.pinMode     ( m.pin, m.mode   ); break;
            //-- Analog I/O -----------------------------------------------------------
            case ardsim.E_ANALOG_WRITE : ardsim.analogWrite ( m.pin, m.value  ); break;
            //-- Communication -----------------------------------------------
            case ardsim.E_PRINT        : ardsim.print       ( m.line );          break;
            case ardsim.E_PRINTLN      : ardsim.println     ( m.line );          break;
            //-- Servo -------------------------------------------------------
            case ardsim.E_SERVO_ATTACH : ardsim.servoAttach ( m.pin );           break;
            case ardsim.E_SERVO_WRITE  : ardsim.servoWrite  ( m.pin, m.value );  break;
            case ardsim.E_SERVO_DETACH : ardsim.servoAttach ( m.pin );           break;
            }
         };
      }
      catch( exception ) {
         console.err( exception );
      }
   },
   idToPin: function( id ) {
      return parseInt( id.substring( id.lastIndexOf( "_" ) + 1 ));
   },
   //-- Digital I/O ----------------------------------------------------------
   digitalChanged: function( checkbox ) {
      console.log( "digitalChanged|checkbox: %o", checkbox );
      let msg =
         "{'verb':"   + ardsim.E_DIGITAL_READ +
         ",'pin':"    + ardsim.idToPin( checkbox.id ) +
         ",'hiOrLo':" + checkbox.checked +
         "}";
      console.log( msg );
      ardsim.ws.send( msg );
   },
   digitalWrite: function( pin, hiOrLo ) {
      console.log( "digitalWrite|pin: %s, hiOrLo: %s", pin, hiOrLo );
      let checkbox = document.getElementById( "ardsim-digital_value_" + pin );
      checkbox.checked = hiOrLo;
   },
   pinMode: function( pin, isOutput ) {
      console.log( "pinMode|pin: %s, isOutput: %s", pin, isOutput );
      let role     = document.getElementById( "ardsim-digital_role_"  + pin );
      let checkbox = document.getElementById( "ardsim-digital_value_" + pin );
      role.textContent = isOutput ? "OUT" : "IN";
      if( isOutput ) {
         checkbox.setAttribute( 'disabled', 'disabled' );
      }
      else {
         checkbox.removeAttribute( 'disabled' );
      }
   },
   //-- Analog I/O -----------------------------------------------------------
   analogChanged: function( slider ) {
      console.log( "analogChanged|slider: %o", slider );
      let pin = ardsim.idToPin( slider.id );
      let msg =
         "{'verb':"  + ardsim.E_ANALOG_READ +
         ",'pin':"   + pin +
         ",'value':" + slider.value +
         "}";
      console.log( msg );
      ardsim.ws.send( msg );
      let label = document.getElementById( "ardsim-analog_value_" + pin );
      label.textContent = slider.value;
   },
   analogWrite: function( pin, value ) {
      console.log( "analogWrite|pin: %s, value: %s", pin, value );
      let slider = document.getElementById( "ardsim-analog_pos_" + pin );
      let label  = document.getElementById( "ardsim-analog_value_" + pin );
      slider.value      = value;
      label.textContent = value;
   },
   //-- Communication --------------------------------------------------------
   print: function( line ) {
      let log  = document.getElementById( "ardsim-log" );
      let allp = log.getElementsByTagName( "p" );
      let p    = null;
      if( allp.length == 0 ) {
         let p = document.createElement( "p" );
         log.appendChild( p );
      }
      else {
         p = allp[allp.length - 1];
      }
      p.textContent = line;
   },
   println: function( line ) {
      let log = document.getElementById( "ardsim-log" );
      let p   = document.createElement( "p" );
      log.appendChild( p );
      p.textContent = line;
   },
   //-- Servo ----------------------------------------------------------------
   servoAttach: function( pin ) {
      console.log( "servoAttach|pin: %s", pin );
      let timeout = document.getElementById( "ardsim-servo_" + pin );
      if( timeout ) {
         let digital_role  = document.getElementById( "ardsim-digital_role_"  + pin );
         let digital_value = document.getElementById( "ardsim-digital_value_" + pin );
         digital_role.textContent = "OUT";
         digital_value.style= "display:none";
         timeout      .style= "display:inline-block";
      }
   },
   servoWrite: function( pin, value ) {
      console.log( "servoWrite|pin: %s, value: %s", pin, value );
      let timeout = document.getElementById( "ardsim-servo_" + pin );
      if( timeout ) {
         timeout.value = value;
      }
   },
   servoDetach: function( pin ) {
      console.log( "servoDetach|pin: %s", pin );
      let timeout = document.getElementById( "ardsim-servo_" + pin );
      if( timeout ) {
         let digital_role  = document.getElementById( "ardsim-digital_role_"  + pin );
         let digital_value = document.getElementById( "ardsim-digital_value_" + pin );
         digital_role.textContent = "---";
         digital_value.style= "display:inline-block";
         timeout      .style= "display:none";
      }
   },
};
