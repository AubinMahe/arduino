package hpms.app.arduinosim;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;

import javax.sound.midi.InvalidMidiDataException;
import javax.sound.midi.MidiEvent;
import javax.sound.midi.MidiSystem;
import javax.sound.midi.MidiUnavailableException;
import javax.sound.midi.Sequence;
import javax.sound.midi.Sequencer;
import javax.sound.midi.ShortMessage;
import javax.sound.midi.Track;

public class ArduinoProxy extends Thread {

   private enum verb_t {

      E_NONE,

      E_PRINT,
      E_PRINTLN,
      E_TONE,
      E_NO_TONE,
      E_PIN_MODE,
      E_DIGITAL_WRITE,
      E_DIGITAL_READ,

      E_EXIT
   }

   private static final int  ACOUSTIC_BASS   = 32;
   private static final int  TEMPO           = 10;
   private static final int  ATTACK_NOTE_ON  = 63;
   private static final int  ATTACK_NOTE_OFF = 30;
   private static final byte HIGH            = 1;
   private static final byte LOW             = 0;
//   private static final byte INPUT           = 0;
   private static final byte OUTPUT          = 1;

   private final Sequencer           _midi = MidiSystem.getSequencer();
   private final ByteBuffer          _recv = ByteBuffer.allocate( 64*1024 );
   private final ByteBuffer          _sent = ByteBuffer.allocate( 5 );
   private final SimulatorController _ctrl;
   private /* */ SocketAddress       _arduino;
   private /* */ DatagramChannel     _channel;

   ArduinoProxy( SimulatorController ctrl ) throws MidiUnavailableException {
      _ctrl = ctrl;
      _midi.open();
   }

   private void print() {
      final int    len   = _recv.getShort();
      final byte[] bytes = new byte[len];
      _recv.get( bytes );
      final String s = new String( bytes );
      _ctrl.serial( s );
   }

   private void println() {
      print();
      _ctrl.serial( "\n" );
   }

   private static double ln2( double x ) {
      return Math.log(x) / Math.log( 2.0 );
   }

   private static int midiNote( int frequency ) {
      return (int)Math.round( 12.0*ln2( frequency / 440.0 ) + 69.0 );
   }

   private Sequence createSequence( int frequency, int duration ) throws InvalidMidiDataException {
      final Sequence sequence = new Sequence( Sequence.PPQ, (int)( 19.0 * TEMPO / 48.0 ));
      final Track    track    = sequence.createTrack();
      final int      channel  = 0;
      ShortMessage shortMessage;
      int tick = 0;
      shortMessage = new ShortMessage();
      shortMessage.setMessage( ShortMessage.PROGRAM_CHANGE, channel, ACOUSTIC_BASS, 0 );
      track.add( new MidiEvent( shortMessage, tick ));
      tick++;
      final int note = midiNote( frequency );
      shortMessage = new ShortMessage();
      shortMessage.setMessage( ShortMessage.NOTE_ON , 0, note, ATTACK_NOTE_ON );
      track.add( new MidiEvent( shortMessage, tick ));
      if( duration > 0 ) {
         tick += duration;
         shortMessage = new ShortMessage();
         shortMessage.setMessage( ShortMessage.NOTE_OFF, 0, note, ATTACK_NOTE_OFF );
         track.add( new MidiEvent( shortMessage, tick ));
      }
      else {
         _midi.setLoopCount( Sequencer.LOOP_CONTINUOUSLY );
      }
      return sequence;
   }

   private void playMidi( int frequency, int duration ) {
      try {
         _midi.setSequence( createSequence( frequency, duration ));
         _midi.start();
         if( duration > 0 ) {
            Thread.sleep( duration );
            noTone();
         }
      }
      catch( final Throwable t ) {
         t.printStackTrace();
      }
   }

   private void tone() {
      /*final byte pin    =*/_recv.get();
      final int  frequency = _recv.getInt();
      final int  duration  = _recv.getInt();
      new Thread(() -> playMidi( frequency, duration )).start();
   }

   private void noTone() {
      /*final byte pin =*/_recv.get();
      _midi.stop();
   }

   void noteOn( int frequency ) {
      new Thread(() -> playMidi( frequency, 0 )).start();
   }

   void noteOff() {
      _midi.stop();
   }

   private void pinMode() {
      final byte    pin     = _recv.get();
      final byte    inOrOut = _recv.get();
      final boolean output  = ( inOrOut == OUTPUT );
      _ctrl.pinMode( pin, output );
   }

   private void digitalWrite() {
      final byte pin     = _recv.get();
      final byte hiOrLo  = _recv.get();
      _ctrl.digitalWrite( pin, hiOrLo == HIGH );
   }

   private void exit() {
      final int exitCode = _recv.getInt();
      System.exit( exitCode );
   }

   void digitalChanged( int pin, boolean selected ) throws IOException {
      _sent.clear();
      _sent.put((byte)verb_t.E_DIGITAL_READ.ordinal());
      _sent.put((byte)pin );
      _sent.put( selected ? HIGH : LOW );
      _sent.flip();
      _channel.send( _sent, _arduino );
   }

   void exit( int exitCode ) {
      try {
         if( _arduino != null ) {
            _sent.clear();
            _sent.put((byte)verb_t.E_EXIT.ordinal());
            _sent.putInt( exitCode );
            _sent.flip();
            _channel.send( _sent, _arduino );
            _channel.close();
         }
      }
      catch( final Throwable t ) {
         t.printStackTrace();
      }
      System.exit( exitCode );
   }

   @Override
   public void run() {
      try {
         _channel = DatagramChannel.open();
         _channel.bind( new InetSocketAddress( 2416 ));
         for(;; Thread.sleep( 20 )) {
            _recv.clear();
            _arduino = _channel.receive( _recv );
            _recv.flip();
            final byte verb = _recv.get();
            if( verb >= 0 && verb < verb_t.values().length ) {
               final verb_t v = verb_t.values()[verb];
               switch( v ) {
               case E_NONE: break;
               case E_PRINT        : print       (); break;
               case E_PRINTLN      : println     (); break;
               case E_TONE         : tone        (); break;
               case E_NO_TONE      : noTone      (); break;
               case E_PIN_MODE     : pinMode     (); break;
               case E_DIGITAL_WRITE: digitalWrite(); break;
               case E_DIGITAL_READ : break;
               case E_EXIT         : exit(); break;
               }
            }
         }
      }
      catch( final IOException | InterruptedException x ) {
         x.printStackTrace();
         System.exit( 1 );
      }
   }
}
