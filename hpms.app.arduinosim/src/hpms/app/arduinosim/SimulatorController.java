package hpms.app.arduinosim;

import java.io.IOException;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.CheckBox;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;

public class SimulatorController {

   @FXML private Label    _direction_1;
   @FXML private Label    _direction_2;
   @FXML private Label    _direction_3;
   @FXML private Label    _direction_4;
   @FXML private Label    _direction_5;
   @FXML private Label    _direction_6;
   @FXML private Label    _direction_7;
   @FXML private Label    _direction_8;
   @FXML private CheckBox _digital_1;
   @FXML private CheckBox _digital_2;
   @FXML private CheckBox _digital_3;
   @FXML private CheckBox _digital_4;
   @FXML private CheckBox _digital_5;
   @FXML private CheckBox _digital_6;
   @FXML private CheckBox _digital_7;
   @FXML private CheckBox _digital_8;
   @FXML private TextArea _serial;

   private Label   []   _direction;
   private CheckBox[]   _digital;
   private ArduinoProxy _proxy;

   void setProxy( ArduinoProxy proxy ) {
      _proxy = proxy;
   }

   @FXML
   public void initialize() {
      _direction = new Label[] {
         _direction_1,
         _direction_2,
         _direction_3,
         _direction_4,
         _direction_5,
         _direction_6,
         _direction_7,
         _direction_8,
      };
      _digital = new CheckBox[] {
         _digital_1,
         _digital_2,
         _digital_3,
         _digital_4,
         _digital_5,
         _digital_6,
         _digital_7,
         _digital_8,
      };
      _serial.setText( "" );
   }

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

   @FXML
   public void noteOn() {
      _proxy.noteOn( 440 );
   }

   @FXML
   public void noteOff() {
      _proxy.noteOff();
   }

   public void pinMode( int pin, boolean output ) {
      Platform.runLater(() -> {
         if( output ) {
            _direction[pin].setText( "OUT" );
            _digital  [pin].setDisable( true );
            _digital  [pin].setStyle( "-fx-opacity: 1" );
         }
         else {
            _direction[pin].setText( "IN" );
            _digital  [pin].setSelected( false );
            _digital  [pin].setDisable( false );
            _digital  [pin].setStyle( "" );
         }
         _digital[pin].setIndeterminate( false );
      });
   }

   public void serial( String line ) {
      _serial.setText( _serial.getText() + line );
   }

   public void digitalWrite( byte pin, boolean status ) {
      Platform.runLater(() -> _digital[pin].setSelected( status ));
   }

   public boolean digitalRead( byte pin ) {
      return _digital[pin].isSelected();
   }
}
