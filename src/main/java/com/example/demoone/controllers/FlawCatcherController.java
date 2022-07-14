package com.example.demoone.controllers;

import com.example.demoone.ChartGenerator;
import com.example.demoone.model.Flaw;
import com.example.demoone.model.Student;
import com.example.demoone.services.AnalyzeService;
import com.example.demoone.util.CSVHelper;
import com.example.demoone.util.FileUtils;
import javafx.beans.property.*;
import javafx.collections.FXCollections;
import javafx.collections.ListChangeListener;
import javafx.collections.ObservableList;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.chart.Chart;
import javafx.scene.control.*;
import javafx.scene.layout.AnchorPane;
import javafx.stage.DirectoryChooser;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.net.URL;
import java.util.*;

public class FlawCatcherController implements Initializable {
    @FXML
    public TextArea errorOutput;
    @FXML
    private Label welcomeText;
    //@FXML
    //private TextArea output;
    @FXML
    private Label statusLabel;
    @FXML
    private ListView<String> files;
    @FXML
    private TextField thresholdPieChartText;

    private String selectedDir = System.getProperty("user.home");
    private String csvHeaders;
    private final IntegerProperty flawCount = new SimpleIntegerProperty(0);
    private final StringProperty status = new SimpleStringProperty("Idle");
    private final StringProperty results = new SimpleStringProperty("");
    private final StringProperty html = new SimpleStringProperty("");
    private final StringProperty errors = new SimpleStringProperty("");
    private final String STATUS_MESSAGE = " %,d flaws found.";
    private final SimpleListProperty<String> filenames =
            new SimpleListProperty<>(FXCollections.observableList(new ArrayList<>()));
    private final Set<String> filenamesSet = new HashSet<>();
    private final Map<String, List<Flaw>> flawsByFilename = new HashMap<>();
    private final SimpleMapProperty<String, List<Flaw>> flaws =
            new SimpleMapProperty<>(FXCollections.observableMap(flawsByFilename));
    private final Map<String, String> rawCSVByFilename = new HashMap<>();

    private final Map<String, String> htmlByFilename = new HashMap<>();

    private boolean isFirstItem;

    //GUI

    @FXML
    AnchorPane middlePane;
    @FXML
    TextArea studentFlaws;
    @FXML
    TableView studentTable = new TableView<>();
    @FXML TableColumn<Student, String> fNameCol = new TableColumn<Student, String>("First Name"),
            cNameCol = new TableColumn<Student, String>("Class Name"),
            fileNameCol = new TableColumn<Student, String>("Path Name");
    ArrayList<Student> students = new ArrayList<Student>();

    @Override
    public void initialize(URL location, ResourceBundle resources) {
        studentTable.getSelectionModel().selectedItemProperty().addListener((obs, oldSelection, newSelection) -> {
            TablePosition pos = (TablePosition) studentTable.getSelectionModel().getSelectedCells().get(0);
            int row = pos.getRow();
            var item = studentTable.getItems().get(row);
            TableColumn col = (TableColumn) studentTable.getColumns().get(2);

            String data = (String) col.getCellObservableValue(item).getValue();
            if (newSelection != null) {
                List<Flaw> flawList = flawsByFilename.get(data);
                String line = "Path Name: " + flawList.get(0).fileName() + "\n";
                if (flawList.get(0).name().equals("No Flaws!")) {
                    line = "No Flaws!";
                } else {
                    for (int i = 0; i < flawList.size(); i++) {
                        line += "Flaw: " + i;
                        line += "\n\tLevel: ";
                        line += flawList.get(i).level();
                        line += "\n\tName: ";
                        line += flawList.get(i).name();
                        line += "\n\tSuggestion: ";
                        line += flawList.get(i).suggest();
                        line += "\n\tLine: ";
                        line += flawList.get(i).line();
                        line += "\n";
                    }
                }
                studentFlaws.setText(line);
                line = "";
            }
        });
    }

    @FXML
    protected void onSimplePieChartClick(ActionEvent event) {
        if (this.students.size() != 0) {
            Chart chart = ChartGenerator.MakePieChartForStudentsWithFlawsAboveThreshold(FXCollections.observableArrayList(this.students), 0);
            this.makeChartWindow(chart);
        }
    }

    @FXML
    protected void onFlawCountPieChartClick(ActionEvent event) {
        if (this.students.size() != 0) {
            Chart chart = ChartGenerator.showFlawsByNumberOfFlaws(FXCollections.observableArrayList(this.students));
            this.makeChartWindow(chart);
        }
    }

    @FXML
    protected void onThresholdPieChartClick(ActionEvent event) {
        if (this.students.size() != 0) {
            int textAsInteger;
            try {
                textAsInteger = Integer.valueOf(thresholdPieChartText.getText());
                Chart chart = ChartGenerator.MakePieChartForStudentsWithFlawsAboveThreshold(FXCollections.observableArrayList(students), textAsInteger);
                this.makeChartWindow(chart);
            } catch (Exception e) {

            }
        }
    }

    @FXML
    protected void onSimpleBarChartClick(ActionEvent event) {
        if (this.students.size() != 0) {
            Chart chart = ChartGenerator.makeBarChart(FXCollections.observableArrayList(this.students));
            this.makeChartWindow(chart);
        }
    }

    private void makeChartWindow(Chart chart) {
        Stage stage = new Stage();
        Group group = new Group(chart);
        Scene scene = new Scene(group);
        stage.setTitle("Chart");
        stage.setHeight(1000.0D);
        stage.setWidth(1000.0D);
        stage.setScene(scene);
        stage.show();
    }


    @FXML
    protected void onCatchFlawsClick(ActionEvent event) throws FileNotFoundException {
        Stage stage = (Stage) ((Node) event.getSource()).getScene().getWindow();
        File file;
        studentTable.getSelectionModel().select(-1);
        if (((Button) event.getSource()).getId().equals("file")) {
            file = chooseFile(stage);
        } else {
            file = chooseDirectory(stage);
        }
        if (file != null) {
            analyzeAndDisplayResults(file);
            generateHTML(file);
        }
    }

    private File chooseDirectory(Stage stage) {
        var fileChooser = new DirectoryChooser();
        fileChooser.setTitle("Choose a file");
        fileChooser.setInitialDirectory(new File(selectedDir));
        return fileChooser.showDialog(stage.getOwner());
    }

    private File chooseFile(Stage stage) {
        var fileChooser = new FileChooser();
        fileChooser.setTitle("Choose a file to analyze");
        fileChooser.setInitialDirectory(new File(selectedDir));
        fileChooser.getExtensionFilters().add(
                new FileChooser.ExtensionFilter("C and C++ files", "*.c", "*.h", "*.cpp"));
        return fileChooser.showOpenDialog(stage.getOwner());
    }

    private void generateHTML(File file) {
        if (file.isDirectory()) {
            Arrays.stream(file.listFiles()).toList().forEach(this::generateHTML);
        } else {
            generateHTMLSingle(file);
        }
    }

    private void generateHTMLSingle(File f) {
        var analyzeService = new AnalyzeService(f.getAbsolutePath(), "--html");
        analyzeService.setOnSucceeded(e -> {
            html.set(String.join("\n", analyzeService.getValue().stream().toList()));
//            BufferedWriter writer = null;
//            try {
//                var filename = f.getName();
//                writer = new BufferedWriter(new FileWriter("/Users/bearmountain/"+filename+".html"));
//                writer.write(html.get());
//                writer.close();
//
//            } catch (IOException ex) {
//                ex.printStackTrace();
//            }
            htmlByFilename.put(f.getAbsolutePath(), html.get());
        });
        analyzeService.start();
    }


    private void analyzeAndDisplayResults(File file) throws FileNotFoundException {
        selectedDir = file.getParentFile().getAbsolutePath();
        results.set("");
        errors.set("");
        filenamesSet.clear();
        isFirstItem = true;
        var analyzeService = new AnalyzeService(file.getAbsolutePath(), "--csv");
        analyzeService.setOnScheduled(event1 -> status.set("Running."));
        analyzeService.setOnFailed(failEvent -> status.set("Failed."));
        analyzeService.setOnSucceeded(e -> {
            status.set("Done.");
            csvHeaders = analyzeService.getValue().stream().findFirst().get();
            results.set(String.join("\n", analyzeService.getValue().stream().skip(1).toList()));
            errors.set(String.join("\n", analyzeService.getErrors()));
            buildRawCSVByFilename(analyzeService.getValue().stream().skip(1).toList());
        });
        analyzeService.getResults().addListener(
                (ListChangeListener<? super String>)
                        change -> {
                            flawCount.set(analyzeService.getResults().size() - 1);
                            while (change.next()) {
                                processResults(Collections.unmodifiableList(change.getAddedSubList()), file);
                            }
                        }
        );
        analyzeService.start();
    }

    private void processResults(List<String> results, File file) {
        try {
            CSVHelper.processResults(results)
                    .forEach(
                            flaw -> flawsByFilename
                                    .compute(flaw.fileName(),
                                            (s, flaws1) -> {
                                                    return Arrays.asList(flaw);
                                            }));
            filenamesSet.addAll(CSVHelper.processResults(results).stream().map(Flaw::fileName).toList());
            filenames.clear();
            filenames.addAll(filenamesSet.stream().sorted().toList());
            loadStudents(file);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    protected Student makeStudent(String s) {
        String fName = "";
        String cName = "";
        String fileName = "";
        String restOfString = "";
        fName = s.substring(s.lastIndexOf("\\")+1, s.indexOf('_', s.lastIndexOf("\\")+1));
        restOfString = s.substring(s.lastIndexOf("\\")+1);
        StringTokenizer tokens = new StringTokenizer(restOfString, "_");
        fileName = s;
        tokens.nextToken();
        tokens.nextToken();
        tokens.nextToken();
        cName = tokens.nextToken("");
        cName = cName.substring(1);
        Student student = new Student(fName, cName, fileName, "", flawsByFilename.get(s));
        return student;
    }

    @FXML protected void loadStudents(File file) throws FileNotFoundException {
        ObservableList<Student> list = FXCollections.observableArrayList();
        studentTable.getColumns().clear();
        boolean found = false;
        for(int i = 0; i < filenames.size(); i++) {
            String s = filenames.get(i);
            for (int j = 0; j < students.size(); j++) {
                if (students.get(j).fNameProperty().toString() != filenames.get(i)) {
                    found = true;
                    students.get(j).setFlaws(flawsByFilename.get(filenames.get(i)));
                }
            }
            if (s.substring(s.length()-4) != ".cpp" && !found) {
                Student student = makeStudent(s);
                students.add(student);
            }
            found = false;
        }
        if (file.isFile()) {
            if (!flawsByFilename.containsKey(file.getAbsolutePath())) {
                if (file.getAbsolutePath().substring(file.getAbsolutePath().length()-4).equals(".cpp")) {
                    List<Flaw> csv = new ArrayList<Flaw>() {
                        {
                            add(new Flaw(file.getAbsolutePath(), 0, "No Flaws!", 0, "", ""));
                        }
                    };
                    flawsByFilename.put(file.getAbsolutePath(), csv);
                    Student student = makeStudent(file.getAbsolutePath());
                    students.add(student);
                }
            }
        } else {
            for (File f : file.listFiles()) {
                if (f.getAbsolutePath().substring(f.getAbsolutePath().length()-4).equals(".cpp")) {
                    if (!flawsByFilename.containsKey(f.getAbsolutePath())) {
                        List<Flaw> csv = new ArrayList<Flaw>() {
                            {
                                add(new Flaw(f.getAbsolutePath(), 0, "No Flaws!", 0, "", ""));
                            }
                        };
                        flawsByFilename.put(f.getAbsolutePath(), csv);
                        Student student = makeStudent(f.getAbsolutePath());
                        students.add(student);
                    }
                }
            }
        }
        list.addAll(students);
        fNameCol.setCellValueFactory(cellData -> cellData.getValue().fNameProperty());
        cNameCol.setCellValueFactory(cellData -> cellData.getValue().cNameProperty());
        fileNameCol.setCellValueFactory(cellData -> cellData.getValue().fileNameProperty());
        for (Student s: list) {
            if (s.getNumFlaws() > 0) {
                System.out.println("TESTTEST");
            }
        }
        studentTable.setItems(list);
        studentTable.setVisible(true);
        studentTable.getColumns().addAll(fNameCol, cNameCol, fileNameCol);
    }

    @FXML
    protected void clearStudents() {
        studentTable.getItems().clear();
        students.clear();
    }

    @FXML
    protected void exportStudents(ActionEvent event) {
        List<Student> noRepeatStudents = ChartGenerator.CombineStudents(students);
        Stage stage = (Stage) ((Node) event.getSource()).getScene().getWindow();
        var fileChooser = new DirectoryChooser();
        fileChooser.setTitle("Choose a directory");
        fileChooser.setInitialDirectory(new File(selectedDir));
        File selectedFile = fileChooser.showDialog(stage);

        for (Student s : noRepeatStudents) {
            var filename = s.fNameProperty().get();
            var key = rawCSVByFilename.keySet().stream().filter(k -> k.contains(filename)).findFirst();
            String csvContents = csvHeaders;
            String htmlContents;
            if(key.isPresent()) {
                csvContents += "\n" + rawCSVByFilename.get(key.get());
                htmlContents = htmlByFilename.get(key.get());
            }else{
                htmlContents = "";
            }
            FileUtils.saveStudentFiles(selectedFile, csvContents, htmlContents, filename);
        }

    }

    private void buildRawCSVByFilename(List<String> results) {

        results.forEach(
                flaw ->
                {

                    rawCSVByFilename.compute(flaw.split(",")[0],
                                    (s, flaws1) -> {
                                        if (flaws1 == null) {
                                            return flaw;
                                        } else {
                                            return flaws1 + "\n" + flaw;
                                        }
                                    }
                            );
                }
        );
    }


}