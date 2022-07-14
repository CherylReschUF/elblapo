package com.example.demoone;

import com.example.demoone.model.Flaw;
import com.example.demoone.model.Student;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.scene.chart.BarChart;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.PieChart;
import javafx.scene.chart.XYChart;
import javafx.scene.chart.CategoryAxis;



import java.util.*;

public class ChartGenerator {
    public static PieChart MakePieChartForStudentsWithFlawsAboveThreshold(ObservableList<Student> students, int threshold){

        int withFlawCount = 0;
        int withoutFlawCount = 0;
        ObservableList<Student> noRepeatsStudents = FXCollections.observableArrayList(CombineStudents(students));
        for (Student student: noRepeatsStudents) {
            if(student.getNumFlaws() > threshold ){
                withFlawCount++;
            }else {
                withoutFlawCount++;
            }
        }
        int total = withFlawCount + withoutFlawCount;
        ObservableList<PieChart.Data> pieChartData = FXCollections.observableArrayList();
        if(threshold > 1 ){
            pieChartData.add(new PieChart.Data("Students with more than " + threshold + " flaws. " + (Math.round(((double) withFlawCount /(double) total) * 100.0)) + "%" ,withFlawCount));
            pieChartData.add(new PieChart.Data("Students with less than " + threshold + " flaws. " + (Math.round(((double) withoutFlawCount /(double) total) * 100.0)) + "%", withoutFlawCount));
        }else {
            pieChartData.add(new PieChart.Data("Students with flaws. " + (Math.round(((double) withFlawCount /(double) total) * 100.0)) + "%",withFlawCount));
            pieChartData.add(new PieChart.Data("Students without flaws. " + (Math.round(((double) withoutFlawCount /(double) total) * 100.0)) + "%", withoutFlawCount));
        }
        PieChart chart = new PieChart(pieChartData);
        chart.setTitle("Students with flaws");
        chart.setMinHeight(900.0D);
        chart.setMinWidth(900.0D);
        return chart;
    }

    public static PieChart showFlawsByNumberOfFlaws(ObservableList<Student> students){
        HashMap<String,Integer> flawsByNumber = new HashMap<String,Integer>();
        ObservableList<Student> noRepeatsStudents = FXCollections.observableArrayList(CombineStudents(students));
        System.out.println(noRepeatsStudents.size());
        for (Student student: noRepeatsStudents) {
            int numFlaws = student.getNumFlaws();
            Integer count = flawsByNumber.get(String.valueOf(numFlaws));
            if(count == null){
                count = Integer.valueOf(0);
            }
            flawsByNumber.put(String.valueOf(numFlaws), count + 1);
        }
        ObservableList<PieChart.Data> pieChartData = FXCollections.observableArrayList();

        for (String value: flawsByNumber.keySet()) {
            pieChartData.add(new PieChart.Data("Students with " + value + " flaws " + Math.round(((double) flawsByNumber.get(value) / (double) noRepeatsStudents.size()) * 100.0) + "%" , flawsByNumber.get(value)));
            System.out.println(flawsByNumber.get(value));
        }
        PieChart chart = new PieChart(pieChartData);
        chart.setTitle("Number of flaws by student");
        chart.setMinHeight(900.0D);
        chart.setMinWidth(900.0D);
        return  chart;

    }

    public static BarChart makeBarChart(ObservableList<Student> students){
        XYChart.Series series = new XYChart.Series();

        ObservableList<Student> noRepeatsStudents = FXCollections.observableArrayList(CombineStudents(students));
        for (Student student: noRepeatsStudents) {
            series.getData().add(new XYChart.Data(student.getNumFlaws(), student.fNameProperty().get() + " " + student.cNameProperty().get()) );
        }
        BarChart<Number,String> chart = new BarChart<Number,String>(new NumberAxis(), new CategoryAxis());
        chart.getData().add(series);
        chart.setTitle("Flaws By student");
        chart.setMinHeight(900.0D);
        chart.setMinWidth(900.0D);
        return chart;
    }

    static BarChart MakeStudentFlawBarChart(Student student){
        XYChart.Series series = new XYChart.Series();

        HashMap<String, Integer> flawsMapped = new HashMap<>();
        for(int i = 0; i < student.getNumFlaws(); i++){
            Flaw flaw = student.getFlaw(i);
            Integer count = flawsMapped.get(flaw.name());
            if(count == null) {
                count = Integer.valueOf(0);
            }
            flawsMapped.put(flaw.name(), count + 1);
        }

        for(String flaw : flawsMapped.keySet()){
            series.getData().add(new XYChart.Data(flaw,flawsMapped.get(flaw)) );
        }
        BarChart<String,Number> chart = new BarChart<String,Number>(new CategoryAxis(), new NumberAxis());
        chart.getData().add(series);
        chart.setTitle("Flaws for " + student.fNameProperty().get() + " " + student.cNameProperty().get());
        chart.setMinHeight(900.0D);
        chart.setMinWidth(900.0D);
        return chart;
    }
    static PieChart MakeStudentFlawPieChart(Student student){
        ObservableList<PieChart.Data> pieChartData = FXCollections.observableArrayList();

        HashMap<String, Integer> flawsMapped = new HashMap<>();
        for(int i = 0; i < student.getNumFlaws(); i++){
            Flaw flaw = student.getFlaw(i);
            Integer count = flawsMapped.get(flaw.name());
            if(count == null) {
                count = Integer.valueOf(0);
            }
            flawsMapped.put(flaw.name(), count + 1);
        }

        for(String flaw : flawsMapped.keySet()){
            pieChartData.add(new PieChart.Data(flaw,flawsMapped.get(flaw)));
        }
        PieChart chart = new PieChart(pieChartData);
        chart.setTitle("Flaws for " + student.fNameProperty().get() + " " + student.cNameProperty().get());
        return chart;
    }

    public static List<Student> CombineStudents(List<Student> students){
        ArrayList<Student> returnStudents = new ArrayList<Student>();
        for(Student student : students){
            returnStudents.add(student.copy());
        }
        boolean done = false;
        while(!done){
            System.out.println("Shrinking Students");
            done = true;
            for(int i = 0; i < returnStudents.size() - 1; i++){
                Student student1 = returnStudents.get(i);
                for(int j = i + 1; j < returnStudents.size(); j++){
                    Student student2 = returnStudents.get(j);
                    if(student1.fNameProperty().get().equals(student2.fNameProperty().get()) && student1.cNameProperty().get().equals(student2.cNameProperty().get()) ) {
                        done = false;
                        if (student1.getFlaw(0).name().equals("No Flaws!")) {
                            returnStudents.remove(student1);
                            i--;
                            break;
                        }
                        if (!student2.getFlaw(0).name().equals("No Flaws!")) {
                            for (int k = 0; k < student2.getNumFlaws(); k++) {
                                student1.addFlaw(student2.getFlaw(k));
                            }
                        }
                        returnStudents.remove(student2);
                        j--;
                    }
                }

            }

        }
        return returnStudents;
    }

}
