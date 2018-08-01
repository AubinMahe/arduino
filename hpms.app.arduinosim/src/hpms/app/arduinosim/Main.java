package hpms.app.arduinosim;

import java.net.URL;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

public class Main extends Application {

   @Override
   public void start( Stage primaryStage ) throws Exception {
      final URL                 fxml   = getClass().getResource( "Simulator.fxml" );
      final FXMLLoader          loader = new FXMLLoader( fxml );
      final Parent              view   = loader.load();
      final SimulatorController ctrl   = loader.getController();
      final ArduinoProxy        proxy  = new ArduinoProxy( ctrl );
      ctrl.setProxy( proxy );
      proxy.start();
      primaryStage.setTitle( "Arduino simulator UI" );
      primaryStage.setScene( new Scene( new BorderPane( view )));
      primaryStage.setOnCloseRequest( e -> proxy.exit( 0 ));
      primaryStage.show();
   }

   public static void main( String[] args ) {
      launch( args );
   }
}
