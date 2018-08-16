package hpms.app.arduinosim;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;

import hpms.fx.angle.AngleControl;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Label;
import javafx.scene.control.Slider;
import javafx.scene.control.TextArea;

public class SimulatorController {

   @FXML private Label        _status;
   @FXML private Label        _direction_0;
   @FXML private Label        _direction_1;
   @FXML private Label        _direction_2;
   @FXML private Label        _direction_3;
   @FXML private Label        _direction_4;
   @FXML private Label        _direction_5;
   @FXML private Label        _direction_6;
   @FXML private Label        _direction_7;
   @FXML private Label        _direction_8;
   @FXML private Label        _direction_9;
   @FXML private Label        _direction_10;
   @FXML private Label        _direction_11;
   @FXML private Label        _direction_12;
   @FXML private Label        _direction_13;
   @FXML private CheckBox     _digital_0;
   @FXML private CheckBox     _digital_1;
   @FXML private CheckBox     _digital_2;

   @FXML private CheckBox     _digital_3;
   @FXML private Slider       _slider_3;
   @FXML private AngleControl _servo_3;

   @FXML private CheckBox     _digital_4;

   @FXML private CheckBox     _digital_5;
   @FXML private Slider       _slider_5;
   @FXML private AngleControl _servo_5;

   @FXML private CheckBox     _digital_6;
   @FXML private Slider       _slider_6;
   @FXML private AngleControl _servo_6;

   @FXML private CheckBox     _digital_7;
   @FXML private CheckBox     _digital_8;

   @FXML private CheckBox     _digital_9;
   @FXML private Slider       _slider_9;
   @FXML private AngleControl _servo_9;

   @FXML private CheckBox     _digital_10;
   @FXML private Slider       _slider_10;
   @FXML private AngleControl _servo_10;

   @FXML private CheckBox     _digital_11;
   @FXML private Slider       _slider_11;
   @FXML private AngleControl _servo_11;

   @FXML private CheckBox     _digital_12;
   @FXML private CheckBox     _digital_13;
   @FXML private Slider       _analogIn_0;
   @FXML private Slider       _analogIn_1;
   @FXML private Slider       _analogIn_2;
   @FXML private Slider       _analogIn_3;
   @FXML private Slider       _analogIn_4;
   @FXML private Slider       _analogIn_5;
   @FXML private TextArea     _serial;
   @FXML private Label        _status1;
   @FXML private Label        _status2;

   private final Map<Byte, Slider>       _sliders = new HashMap<>();
   private final Map<Byte, AngleControl> _servos  = new HashMap<>();
   private /* */ Label   []              _direction;
   private /* */ CheckBox[]              _digital;
   private /* */ Slider  []              _analogIn;
   private /* */ ArduinoProxy            _proxy;
   private /* */ long                    _msgCount;

   void setProxy( ArduinoProxy proxy ) {
      _proxy = proxy;
      try {
         final var lines = Files.readAllLines( Paths.get( "instructions.txt" ));
         _status1.setText( lines.get( 0 ));
         _status2.setText( lines.get( 1 ));
      }
      catch( final Throwable t ) {/**/}
   }

   @FXML
   public void initialize() {
      _direction = new Label[] {
         _direction_0,
         _direction_1,
         _direction_2,
         _direction_3,
         _direction_4,
         _direction_5,
         _direction_6,
         _direction_7,
         _direction_8,
         _direction_9,
         _direction_10,
         _direction_11,
         _direction_12,
         _direction_13,
      };
      _digital = new CheckBox[] {
         _digital_0,
         _digital_1,
         _digital_2,
         _digital_3,
         _digital_4,
         _digital_5,
         _digital_6,
         _digital_7,
         _digital_8,
         _digital_9,
         _digital_10,
         _digital_11,
         _digital_12,
         _digital_13,
      };
      _analogIn = new Slider[] {
         _analogIn_0,
         _analogIn_1,
         _analogIn_2,
         _analogIn_3,
         _analogIn_4,
         _analogIn_5,
      };
      _sliders.put( Byte.valueOf((byte) 3 ), _slider_3  );
      _sliders.put( Byte.valueOf((byte) 5 ), _slider_5  );
      _sliders.put( Byte.valueOf((byte) 6 ), _slider_6  );
      _sliders.put( Byte.valueOf((byte) 9 ), _slider_9  );
      _sliders.put( Byte.valueOf((byte)10 ), _slider_10 );
      _sliders.put( Byte.valueOf((byte)11 ), _slider_11 );
      _servos .put( Byte.valueOf((byte) 3 ), _servo_3  );
      _servos .put( Byte.valueOf((byte) 5 ), _servo_5  );
      _servos .put( Byte.valueOf((byte) 6 ), _servo_6  );
      _servos .put( Byte.valueOf((byte) 9 ), _servo_9  );
      _servos .put( Byte.valueOf((byte)10 ), _servo_10 );
      _servos .put( Byte.valueOf((byte)11 ), _servo_11 );
      byte p = 0;
      for( final Slider pb : _analogIn ) {
         final byte pin = p++;
         pb.valueProperty().addListener(( o, b, a ) -> analog( pin, pb.getValue()));
      }
      _serial.setText( "" );
   }

   @FXML
   public void reset() throws IOException {
      _proxy.reset();
   }

   public void onMsgRecv() {
      Platform.runLater(() -> {
         ++_msgCount;
         _status.setText( "Arduino simulator : message(s) reçu : " + _msgCount );
      });
   }

   @FXML
   public void noteOn() {
      _proxy.noteOn( 440 );
   }

   @FXML
   public void noteOff() {
      _proxy.noteOff();
   }

   //-- Digital I/O ----------------------------------------------------------

   @FXML
   public void digital( ActionEvent e ) throws IOException {
      int pin = -1;
      for( int p = 0; p < _digital.length; ++p ) {
         if( e.getSource() == _digital[p] ) {
            pin = p;
            break;
         }
      }
      _proxy.digitalChanged( pin, _digital[pin].isSelected());
   }

   public boolean digitalRead( byte pin ) {
      return _digital[pin].isSelected();
   }

   public void digitalWrite( byte pin, boolean status ) {
      Platform.runLater(() -> _digital[pin].setSelected( status ));
   }

   public void pinMode( int pin, boolean output ) {
      Platform.runLater(() -> {
         String text = _direction[pin].getText();
         text = text.substring( 0, text.length() - 3);
         if( output ) {
            _direction[pin].setText( text + "OUT" );
            _digital  [pin].setDisable( true );
            _digital  [pin].setStyle( "-fx-opacity: 1" );
         }
         else {
            _direction[pin].setText( text + "IN" );
            _digital  [pin].setSelected( false );
            _digital  [pin].setDisable( false );
            _digital  [pin].setStyle( "" );
         }
         _digital[pin].setIndeterminate( false );
      });
   }

   //-- Analog I/O -----------------------------------------------------------

   void analog( byte pin, double value ) {
      try {
         _proxy.analogChanged( pin, (int)( 1024 * value ));
      }
      catch( final IOException e ) {
         e.printStackTrace();
      }
   }

   public int analogRead( byte pin ) {
      return (int)( 1024 * _analogIn[pin].getValue());
   }

   public void analogWrite( byte pin, int value ) {
      Platform.runLater(() -> {
         final Slider slider = _sliders.get( pin );
         slider.setVisible( true  );
         _digital[ pin ].setVisible( false );
         slider.setValue( value );
      });
   }

   //-- Communication --------------------------------------------------------

   public void serial( String line ) {
      _serial.setText( _serial.getText() + line );
   }

   //-- Servo ----------------------------------------------------------------

   public void servoAttach( byte pin ) {
      Platform.runLater(() -> {
         final AngleControl angle = _servos.get( pin );
         if( angle != null ) {
            String text  = _direction[pin].getText();
            text = text.substring( 0, text.length() - 3);
            _direction[pin].setText( text + "OUT" );
            _digital  [pin].setVisible( false );
            angle          .setVisible( true  );
         }
      });
   }

   public void servoWrite( byte pin, int value ) {
      Platform.runLater(() -> {
         final AngleControl angle = _servos.get( pin );
         if( angle != null && angle.isVisible()) {
            angle.setAngle( value );
         }
      });
   }

   public void servoDetach( byte pin ) {
      Platform.runLater(() -> {
         final AngleControl angle = _servos.get( pin );
         if( angle != null ) {
            String text = _direction[pin].getText();
            text = text.substring( 0, text.length() - 3);
            _direction[pin].setText( text + "---" );
            _digital  [pin].setVisible( true  );
            angle          .setVisible( false );
         }
      });
   }
}
