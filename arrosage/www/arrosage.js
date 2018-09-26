var arrosage  = null;
var http      = new XMLHttpRequest();

function instant_to_hhmm( instant ) {
   var hh = instant.heure;
   if( hh < 10 ) {
      hh = '0' + hh;
   }
   var mm = instant.minute;
   if( mm < 10 ) {
      mm = '0' + mm;
   }
   var hhmm = hh + ':' + mm;
   for( var i = 1; i < arguments.length; ++i ) {
      document.getElementById( arguments[i] ).value = hhmm;
   }
   return hhmm;
}

function hhmm_to_instant( id ) {
   var hm = document.getElementById( id ).value.split(':');
   return {
      heure : parseInt( hm[0] ),
      minute: parseInt( hm[1] )
   };
}

function get_duree( id ) {
   return parseInt( document.getElementById( id ).value );
}

function set_duree( duree, id ) {
   document.getElementById( id ).value = duree;
}

function lire_la_configuration() {
   http.onreadystatechange = function( event ) {
      if( this.readyState === XMLHttpRequest.DONE ) {
         if( this.status === 200 ) {
            try {
               console.log( this.responseText );
               var configuration = JSON.parse( this.responseText );
               if( configuration.code === 0 ) {
                  var arrosage = configuration.data.arrosage;
                  instant_to_hhmm( configuration.data.arrosage.horloge,
                     "heure", "heure_de_la_configuration" );
                  document.getElementById( "est_en_marche" ).value = arrosage.est_en_marche;
                  for( var pin = 0; pin < arrosage.vannes.length; ++pin ) {
                     var vanne = arrosage.vannes[pin];
                     var vm    = "vanne-" + pin + "-matin-";
                     var vs    = "vanne-" + pin + "-soir-";
                     instant_to_hhmm( vanne.matin.ouverture, vm + "ouverture" );
                     set_duree      ( vanne.matin.duree    , vm + "duree" );
                     instant_to_hhmm( vanne.soir .ouverture, vs + "ouverture"  );
                     set_duree      ( vanne.soir .duree    , vs + "duree"  );
                  }
                  document.getElementById( "status-line" ).innerText = "Configuration chargée."; 
               }
               else {
                  document.getElementById( "status-line" ).innerText = configuration.msg; 
               }
            }
            catch( x ) {
               console.error( x );
               document.getElementById( "status-line" ).innerText = "Erreur interne !";
            }
         }
      }
   };
   http.open( 'POST', location.host, true );
   http.setRequestHeader( "Content-type", "application/json" );
   http.send( JSON.stringify({commande: "Lire la configuration"}));
}

function send( cmd ) {
   console.log( "send: -%o-", cmd );
   http.onreadystatechange = function( event ) {
      if( this.readyState === XMLHttpRequest.DONE ) {
         if( this.status === 200 ) {
            try {
               console.log( this.responseText );
               var configuration = JSON.parse( this.responseText );
               if( configuration.code === 0 ) {
                  document.getElementById( "status-line" ).innerText = "Commande exécutée.";
               }
               else if( configuration.msg ) {
                  document.getElementById( "status-line" ).innerText = configuration.msg; 
               }
               else {
                  document.getElementById( "status-line" ).innerText = "Erreur interne !"; 
               }
            }
            catch( x ) {
               console.error( x );
               document.getElementById( "status-line" ).innerText = "Erreur interne !";
            }
         }
      }
   };
   http.open( 'POST', location.host, true );
   http.setRequestHeader( "Content-type", "application/json" );
   http.send( JSON.stringify( cmd ));
}

function demarrer_ou_arreter() {
   var on_off = document.getElementById( "on-off" ).checked;
   send({commande: "Démarrer ou arrêter", argument: {demarrer: on_off }});   
}

function ligne_de_journal( event ) {
   var newRow = document.createElement('tr');
   var newTs  = document.createElement('td');
   var newPin = document.createElement('td');
   var newEvt = document.createElement('td');
   newTs .innerText = instant_to_hhmm( event.instant );
   newPin.innerText = event.pin;
   newEvt.innerText = event.etat ? "Ouverture" : "Fermeture";
   newRow.appendChild( newTs  );
   newRow.appendChild( newPin );
   newRow.appendChild( newEvt );
   return newRow;
}

var auto_test_timer = 0;

function lire_l_auto_test() {
   http.onreadystatechange = function( event ) {
      if( this.readyState === XMLHttpRequest.DONE ) {
         if( this.status === 200 ) {
            try {
               console.log( this.responseText );
               var at = JSON.parse( this.responseText );
               if( at.code === 0 ) {
                  var minutes = 60.0*at.data.instant.heure + at.data.instant.minute;
                  var percent = (100.0*( minutes / (24.0*60.0))).toFixed(0);
                  if( ! at.data.autotest ) {
                     clearInterval( auto_test_timer )
                     percent = 100;
                  }
                  var pWidget = document.getElementById('progress-widget');
                  var spans   = pWidget.getElementsByTagName('span');
                  spans[0].style.width = percent + "%";
                  spans[1].innerText = percent + " %";
                  var events = document.getElementById('auto-test-events');
                  var trs    = events.getElementsByTagName('tr');
                  for( var i = 0, count = at.data.events.length - trs.length; i < count; ++i ) {
                     events.appendChild( ligne_de_journal( at.data.events[count-i-1] ));
                  }
               }
               else {
                  document.getElementById( "status-line" ).innerText = configuration.msg; 
               }
            }
            catch( x ) {
               console.error( x );
               document.getElementById( "status-line" ).innerText = "Erreur interne !";
            }
         }
      }
   };
   http.open( 'POST', location.host, true );
   http.setRequestHeader( "Content-type", "application/json" );
   http.send( JSON.stringify({commande: "Lire l'auto-test"}));
}

function auto_test( on ) {
   send({commande: "Auto-test", argument: {demarrer: on }});
   if( on ) {
      auto_test_timer = setInterval( lire_l_auto_test, 1000 );
   }
}

function mettre_a_l_heure() {
   send({commande: "Mettre à l'heure", argument: hhmm_to_instant( "heure" )});   
}

function lire_l_etat_des_vannes() {
   http.onreadystatechange = function( event ) {
      if( this.readyState === XMLHttpRequest.DONE ) {
         if( this.status === 200 ) {
            try {
               console.log( this.responseText );
               var configuration = JSON.parse( this.responseText );
               if( configuration.code === 0 ) {
                  var etat_des_vannes = configuration.data.etat_des_vannes;
                  for( var pin = 0; pin < etat_des_vannes.length; ++pin ) {
                     document.getElementById( "pin-" + pin + "-state" ).innerText =
                        etat_des_vannes[pin] ? "Ouverte" : "Fermée"; 
                  }
               }
               else {
                  document.getElementById( "status-line" ).innerText = configuration.msg; 
               }
            }
            catch( x ) {
               console.error( x );
               document.getElementById( "status-line" ).innerText = "Erreur interne !";
            }
         }
      }
   };
   http.open( 'POST', location.host, true );
   http.setRequestHeader( "Content-type", "application/json" );
   http.send( JSON.stringify({commande: "Lire l'état des vannes"}));
}

function ouvrir_ou_fermer_les_vannes() {
   var msg = {
      commande: "Ouvrir ou fermer les vannes",
      argument: {
         les_vannes: [],
      },
   };
   for( var pin = 0; pin < 4; ++pin ) {
      var etat = parseInt( document.getElementById( "of-vanne-" + pin + "-etat" ).value );
      if( etat != 0 ) {
         msg.argument.les_vannes.push({ pin : pin, etat: etat });
      }
   }
   if( msg.argument.les_vannes.length ) {
      send( msg );   
   }
}

function charger_une_configuration() {
   var les_vannes = [];
   for( var pin = 0; pin < 4; ++pin ) {
      var vm = "vanne-" + pin + "-matin-";
      var vs = "vanne-" + pin + "-soir-";
      les_vannes.push({
         matin: {
            ouverture: hhmm_to_instant( vm + "ouverture" ),
            duree    : get_duree      ( vm + "duree"     )
         },
         soir : {
            ouverture: hhmm_to_instant( vs + "ouverture"  ),
            duree    : get_duree      ( vs + "duree"      )
         }
      });
   }
   var cfg = {
      arrosage: {
         est_en_marche: document.getElementById( "est_en_marche" ).checked,
         horloge      : hhmm_to_instant( "heure_de_la_configuration" ),
         vannes       : les_vannes
      }
   };
   send({commande: "Charger une configuration", argument: cfg});
}

function expand_or_collapse( p ) {
   var div = p.nextElementSibling;
   if( div.style.display != 'block' ) {
      div.style.display = 'block';
      var panes = document.getElementsByClassName( 'pane' );
      for( var i = 0; i < panes.length; ++i ) {
         var pane = panes[i];
         if( pane.firstElementChild != p ) {
            pane.getElementsByTagName('div')[0].style.display = 'none';
         }
      }
   }
   else {
      div.style.display = 'none';
   }
}
