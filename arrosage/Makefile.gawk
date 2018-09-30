/.+\.o/ {
   split( $2, parts, "/" )
   SRC_PRFX = PREFIX
   count = length( parts )
   for( i in parts ) {
      if( i < count ) {
         SRC_PRFX = SRC_PRFX "/" parts[i]
      }
   }
   print SRC_PRFX "/" $0
   next
}
 { print }
