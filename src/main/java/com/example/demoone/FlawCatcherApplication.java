package com.example.demoone;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.stage.Stage;
import java.io.IOException;
import java.net.URL;

public class FlawCatcherApplication extends Application {
    @Override
    public void start(Stage stage) throws IOException {
        URL fxmlLocation = getClass().getResource("hello-view.fxml");
        System.out.println(fxmlLocation);
        FXMLLoader fxmlLoader = new FXMLLoader(fxmlLocation);
        Scene scene = new Scene(fxmlLoader.load());
        stage.setTitle("Flaw Catcher");
        stage.setScene(scene);
        stage.show();
    }

    public static void main(String[] args) {
        launch();
    }
}