package com.example.demoone.analyzers;

import com.example.demoone.services.SystemCommandService;
import com.example.demoone.util.ProgramDirectoryUtilities;
import javafx.collections.ObservableList;
import javafx.concurrent.Task;

import java.util.ArrayList;
import java.util.List;

public class FlawFinderDriver extends Task<List<String>> {
    private final String inputPath;
    private final String currentDirectory;
    private final String format;
    private final ObservableList<String> results;
    private final ObservableList<String> errors;

    private static final String flawFinderRelativePath = "/python/flawfinder.py";
    private static final String pythonRelativePath = "/python/3.9/python";

    public FlawFinderDriver(String inputPath, ObservableList<String> results, ObservableList<String> errors, String format) {
        this.inputPath = inputPath;
        this.currentDirectory = ProgramDirectoryUtilities.getProgramDirectory();
        this.results = results;
        this.errors = errors;
        this.format = format;
    }

    @Override
    public List<String> call() {
        var commands = new ArrayList<String>();
        commands.add(currentDirectory + pythonRelativePath);
        commands.add(currentDirectory + flawFinderRelativePath);
        commands.add(format);
        commands.add("-i"); //may be a performance hit
        commands.add(inputPath);
        SystemCommandService.runCommand(commands, results, errors);
        return results;
    }

}
