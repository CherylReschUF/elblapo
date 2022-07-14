package com.example.demoone.services;

import javafx.application.Platform;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class SystemCommandService {
    public static void runCommand(ArrayList<String> commands, List<String> results, List<String> errors) {
        ProcessBuilder processBuilder = new ProcessBuilder();
        String os = System.getProperty("os.name");

        if (os.contains("Mac")) {
            commands.add(0,commands.remove(0).replace("/python/", "/python/macOS/"));
            commands.add(0, "/usr/bin/arch");
            commands.add(1,"-x86_64");
        } else {
            commands.add(0,commands.remove(0).replace("/python/", "/python/win/"));
            var env = processBuilder.environment();
            env.put("PYTHONUTF8","1");
        }
        processBuilder.command(commands);
        try {
            Process process = processBuilder.start();
            BufferedReader reader = process.inputReader();
            String line;
            Thread errorReaderThread = buildErrorReaderThread(errors, process);
            errorReaderThread.start();
            while ((line = reader.readLine()) != null) {
                String finalLine = line.trim();
                if(!finalLine.isEmpty()) {
                    Platform.runLater(() -> results.add(finalLine));
                }            }
            errorReaderThread.join();
            int exitVal = process.waitFor();
            if (exitVal == 0) {
                System.out.println("Success!");
            } else {
                System.out.println("Failed with exit code: " + exitVal);
            }
            System.out.println(results);

        } catch (IOException | InterruptedException e) {
            e.printStackTrace();
        }
    }

    private static Thread buildErrorReaderThread(List<String> errors, Process process) {
        return new Thread(() -> {
            try {
                BufferedReader errReader = process.errorReader();
                String error;
                while ((error = errReader.readLine()) != null) {
                    String finalLine = error;
                    Platform.runLater(() -> errors.add(finalLine));
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        });
    }
}
