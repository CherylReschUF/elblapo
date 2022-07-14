package com.example.demoone.model;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.collections.FXCollections;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Student {

    private final StringProperty fName = new SimpleStringProperty();
    private final StringProperty cName = new SimpleStringProperty();
    private final StringProperty fileName = new SimpleStringProperty();
    private List<Flaw> results = Collections.EMPTY_LIST;
    private File csv = new File("");
    String path;

    public Student() {
        this("", "","", "", Collections.EMPTY_LIST);
    }

    public Student(String fName, String cName, String fileName, String path, List<Flaw> results) {
        fNameProperty().set(fName);
        cNameProperty().set(cName);
        fileNameProperty().set(fileName);
        this.results = results;
        this.path = path;
    }

    public Student copy(){
        ArrayList<Flaw> newResults = new ArrayList<>();
        for(Flaw flaw : results){
            newResults.add(flaw);
        }
        return new Student(fName.get(),cName.get(),fileName.get(), path, newResults);
    }

    public StringProperty fNameProperty() {
        return fName ;
    }

    public StringProperty cNameProperty() {
        return cName ;
    }

    public StringProperty fileNameProperty() {
        return fileName ;
    }

    public String getFlaws() {
        String s = "";
        for (Flaw f : results) {
            s += f.line();
        }
        return s;
    }

    public String getFlawsFormatted(){
        String returnString = "";
        for(Flaw f : results){
            if (f.name().equals("")) {
                return "No Flaws!";
            }
            returnString += "Level " + f.level() + " flaw on line " + f.line() + ": " + f.name() + " " + f.warning();
            if(!(f.suggest() != null || f.suggest().equals(""))){
                returnString += "/n suggested fix: " + f.suggest();
            }
            returnString += "\n";
        }
        return returnString;
    }

    public Flaw getFlaw(int index) {
        return (Flaw)this.results.get(index);
    }

    public void setFlaws(List<Flaw> list) {
        results = list;
    }

    public void addFlaw(Flaw flaw) {
        ArrayList<Flaw> asArray = new ArrayList(this.results);
        asArray.add(flaw);
        this.results = asArray;
    }

    public int getNumFlaws() {
        if  (results.get(0).name().equals("No Flaws!")) {
            return 0;
        } else {
            return this.results.size();
        }
    }
}
