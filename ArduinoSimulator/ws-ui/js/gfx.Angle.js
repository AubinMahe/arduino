if( typeof Math.toRadians === 'undefined' ) {
   Math.toRadians = function( degrees ) {
      return ( degrees * 2.0*Math.PI ) / 360.0;
   };
}

var gfx = {
   
   Angle: function( canvas ) {
      this.coords = [
         {x:   4*Math.cos( Math.toRadians( 180 )), y: 4*Math.sin( Math.toRadians( 180 ))},
         {x:   4*Math.cos( Math.toRadians( 165 )), y: 4*Math.sin( Math.toRadians( 165 ))},
         {x:   4*Math.cos( Math.toRadians( 150 )), y: 4*Math.sin( Math.toRadians( 150 ))},
         {x:   4*Math.cos( Math.toRadians( 135 )), y: 4*Math.sin( Math.toRadians( 135 ))},
         {x:   4*Math.cos( Math.toRadians( 120 )), y: 4*Math.sin( Math.toRadians( 120 ))},
         {x:   4*Math.cos( Math.toRadians( 105 )), y: 4*Math.sin( Math.toRadians( 105 ))},
         {x:   4*Math.cos( Math.toRadians( 110 )), y: 4*Math.sin( Math.toRadians( 110 ))},
         {x:   4*Math.cos( Math.toRadians(  85 )), y: 4*Math.sin( Math.toRadians(  85 ))},
         {x:16+2*Math.cos( Math.toRadians(  85 )), y: 2*Math.sin( Math.toRadians(  85 ))},
         {x:16+2*Math.cos( Math.toRadians(  75 )), y: 2*Math.sin( Math.toRadians(  75 ))},
         {x:16+2*Math.cos( Math.toRadians(  60 )), y: 2*Math.sin( Math.toRadians(  60 ))},
         {x:16+2*Math.cos( Math.toRadians(  45 )), y: 2*Math.sin( Math.toRadians(  45 ))},
         {x:16+2*Math.cos( Math.toRadians(  30 )), y: 2*Math.sin( Math.toRadians(  30 ))},
         {x:16+2*Math.cos( Math.toRadians(  15 )), y: 2*Math.sin( Math.toRadians(  15 ))},
         {x:16+2*Math.cos( Math.toRadians(   0 )), y: 2*Math.sin( Math.toRadians(   0 ))},
         {x:16+2*Math.cos( Math.toRadians( -15 )), y: 2*Math.sin( Math.toRadians( -15 ))},
         {x:16+2*Math.cos( Math.toRadians( -30 )), y: 2*Math.sin( Math.toRadians( -30 ))},
         {x:16+2*Math.cos( Math.toRadians( -45 )), y: 2*Math.sin( Math.toRadians( -45 ))},
         {x:16+2*Math.cos( Math.toRadians( -60 )), y: 2*Math.sin( Math.toRadians( -60 ))},
         {x:16+2*Math.cos( Math.toRadians( -75 )), y: 2*Math.sin( Math.toRadians( -75 ))},
         {x:16+2*Math.cos( Math.toRadians( -85 )), y: 2*Math.sin( Math.toRadians( -85 ))},
         {x:   4*Math.cos( Math.toRadians( -85 )), y: 4*Math.sin( Math.toRadians( -85 ))},
         {x:   4*Math.cos( Math.toRadians( -90 )), y: 4*Math.sin( Math.toRadians( -90 ))},
         {x:   4*Math.cos( Math.toRadians(-105 )), y: 4*Math.sin( Math.toRadians(-105 ))},
         {x:   4*Math.cos( Math.toRadians(-120 )), y: 4*Math.sin( Math.toRadians(-120 ))},
         {x:   4*Math.cos( Math.toRadians(-135 )), y: 4*Math.sin( Math.toRadians(-135 ))},
         {x:   4*Math.cos( Math.toRadians(-150 )), y: 4*Math.sin( Math.toRadians(-150 ))},
         {x:   4*Math.cos( Math.toRadians(-165 )), y: 4*Math.sin( Math.toRadians(-165 ))},
         {x:   4*Math.cos( Math.toRadians( 180 )), y: 4*Math.sin( Math.toRadians( 180 ))},
      ];
      this.gc = canvas.getContext( '2d' );
      
      this.value = 0;
   },
};

gfx.Angle.prototype.render = function() {
   let scale = this.gc.canvas.width / 38.0;
   this.gc.clearRect( 0, 0, this.gc.canvas.width, this.gc.canvas.height );
   this.gc.save();
   this.gc.scale( scale, -scale );
   this.gc.translate( 19.0, -19.0 );
   this.gc.rotate( Math.toRadians( this.value ));
   this.gc.lineWidth = 0.25;
   this.gc.beginPath();
   this.gc.moveTo( this.coords[0].x, this.coords[0].y );
   for( let c = 0; c < this.coords.length; ++c ) {
      this.gc.lineTo( this.coords[c].x, this.coords[c].y );
   }
   this.gc.moveTo(  2.0, 0 );
   this.gc.arc   (    0, 0, 2.0, 0, 2.0*Math.PI );
   this.gc.moveTo( 14.8, 0 );
   this.gc.arc   ( 14.0, 0, 0.8, 0, 2.0*Math.PI );
   this.gc.moveTo( 11.8, 0 );
   this.gc.arc   ( 11.0, 0, 0.8, 0, 2.0*Math.PI );
   this.gc.moveTo(  8.8, 0 );
   this.gc.arc   (  8.0, 0, 0.8, 0, 2.0*Math.PI );
   this.gc.stroke();
   this.gc.restore();
   this.gc.textAlign    = "left";
   this.gc.textBaseline = "top";
   this.gc.font         = "11px monospace lighter";
   this.gc.moveTo( 4, 4 );
   this.gc.strokeText( this.value.toFixed(0) + '°', 4, 4 );
   this.gc.closePath();
};
