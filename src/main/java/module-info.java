module com.example.demoone {
    requires javafx.controls;
    requires javafx.fxml;
    requires javafx.web;
    requires java.sql;
    requires com.dlsc.formsfx;
    requires org.kordamp.bootstrapfx.core;
    requires eu.hansolo.tilesfx;
    requires commons.csv;
    requires java.desktop;

    opens com.example.demoone to javafx.fxml;
    exports com.example.demoone;
    exports com.example.demoone.controllers;
    opens com.example.demoone.controllers to javafx.fxml;
}